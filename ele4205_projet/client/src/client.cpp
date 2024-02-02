#include "../include/client.h"
#include "../include/faces.h"



using namespace std;
using namespace cv;

Client::Client(){}


void Client::connection(int port){
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket == -1){
        cout<<"Erreur dans la creation du socket. \n";
        return;
    }
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("192.168.7.2");
    serverAddress.sin_port = htons(port);

    if(connect(clientSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) != 0 )
    {
        cout << "Connexion echoue \n";
        return;
    }
    else {
        cout << "Connexion reussi \n";
    
    }
}

void Client::menu(){

    cout<<"Choisir une fonctionnalite du programme :"<<endl;
    cout<< "'W' : Changer le format de l'image en JPEG"<<endl;
    cout<<"'Q' : Changer le format de l'image en PNG"<<endl;
    cout<<"'1' : Changer la résolution de l'image à 160 x 120"<<endl;
    cout<<"'2' : Changer la résolution de l'image à 352 x 288"<<endl;
    cout<<"'3' : Changer la résolution de l'image à 960 x 720"<<endl;
    cout<<"'4' : Changer la résolution de l'image à 1280 x 720"<<endl;
    cout<<"'t' : Effectuer le mode apprentissage"<<endl;
    cout<<"'r' : Effectuer le mode reconnaissance"<<endl;
    cout<<"'ESC' : Fermer le programme"<<endl;
}




void Client::sendAndReceiveMessage(){

    int indexImage = 0; //Variable inclut dans les noms des images sauvegardees pour les differenciés 
    int flag = 0;       
    int pid = 1;        //Variable du fork
    ostringstream oss;  //variable pour ajouter des int a des strings
    int modeChoice = 0; //Variable inclut pour différencier entre le mode normal, apprentissage et reconnaissance
    int imageTraining = 0; //Variable qui décide si on sauvegarde des images dans un vecteur lors du mode d'apprentissage
    ifstream file("../DatasetFace.yml");
    
    bool fileExist = false; //Variable utilisé pour savoir si le fichier DatasetFace.yml existe ou non
    
    //On demande la connection du serveur et on affiche les commandes possibles du client
    connection(PORT);
    menu();

    while(1){
        if(file.good()){
            fileExist = true;
        }

         ssize_t messageBytes = recv(clientSocket, &serverMessage, sizeof(uint32_t), 0);
         if(messageBytes != sizeof(uint32_t)){break;}
          while(serverMessage == IDOWN){
            messageBytes = recv(clientSocket, &serverMessage, sizeof(uint32_t), 0);
            char key2 = waitKey(30);
            if(key2 == (char)27){
                message = ELE4205_QUIT;
                send(clientSocket, &message, sizeof(uint32_t),0);
                flag = 1;
                break;
                }
            }

         if(flag == 1){break;}

        // Réception de la taille et des bytes de l'image envoyé par le serveur
        buffer.clear();
        message = 0;
        recv(clientSocket, &bufferSize, sizeof(uint),0);
        for(int i = 0; i<bufferSize; i++){
            recv(clientSocket, &imageData, sizeof(imageData),0);
            buffer.push_back(imageData);
        }
        
        // Encodage de l'image et enregristement des images de détections de visage dans un vecteur lors du mode apprentissage
        imageFrame = imdecode(buffer, IMREAD_COLOR);
        if(imageTraining == 1){
            faceImg = detectVisageApprentissage(imageFrame);
            if(!faceImg.empty()){
                trainingVector.push_back(faceImg);
            }
        }

        if(imageFrame.empty()){
            cout<<"Erreur dans le decodage de l'image"<<endl;
            close(clientSocket);
            break;
        }

        // On effectue un fork lorsque le boutton de l'odroid-C2 est pesé (message du serveur)
        if(serverMessage == PUSHB){
            indexImage = indexImage+1;
            pid = fork();
            imageTraining = 0;
            tempMode = modeChoice;
            modeChoice = 0;
        }
        
        // On debute le child process
        if(pid == 0){
            
            // On demande la connection avec le serveur buzzer
            connection(PORT2);
           
            //Enregistrement des images prises une fois le boutton pesé
            oss << indexImage;
            string imagePath = "../Image/" + oss.str() +".png";
            imwrite(imagePath,imageFrame);
            oss.clear();
            oss.str("");

            //Détection des visages
            vector<Rect> faces = detectVisagesReconnaissance(imagePath,indexImage);
 
            // Fonctionnement du mode reconnaissance et apprentissage ainsi que l'envoie du message au serveur buzzer
            switch(tempMode){

                // Mode Normal
                case 0 :
                    buzzerMessage = DEFAULT;
                    break; 

                // Mode reconnaissance, on affiche le nom des personnes reconnu
                case 1 :
                    label = reconnaissance(imagePath,faces);
                    numberOfFaceRecognize = 0;
                    for(int i = 0; i<label.size(); i++){
                        if(getNameFromId(label[i])!="UNKNOWN USER"){
                            numberOfFaceRecognize++;
                        }
                    }
                    if(numberOfFaceRecognize>0){
                        buzzerMessage = KNOWN;
                    }
                    else{
                        buzzerMessage = UNKNOWN;
                    }
                    cout << "Sortie du mode reconnaissance" << endl;
                    break;
                
                //Mode Apprentissage, on entraine le model avec les images inclut dans le vecteur d'image 
                case 2 :
                    cout << "Debut du mode apprentissage" << endl;
                    cout<<"Entrer votre nom"<<endl;
                    cin >> name;
                    if(readFiles(name) == false){
                        namesFile(name);
                    }

                    if(fileExist){
                        cout<<"MISE A JOUR DU MODEL DE RECONNAISSANCE"<<endl;
                        updateModel(trainingVector,name);
                    }
                    else{
                        cout<<"CREATION DU MODEL DE RECONNAISSANCE"<<endl;
                        preTrainingModel(trainingVector,name);
                    }
                    
                    buzzerMessage = LEARN;
                    name ="";
                    trainingVector.clear();
                    cout << "Sortie du mode apprentissage" << endl;
                    break;
            }
            cout << "---------------------------------" << endl;
            cout<<"\n \n"<<endl;
            menu();
            send(clientSocket, &buzzerMessage, sizeof(uint32_t),0);
            close(clientSocket);
            exit(0);
               
        }
        else {
            int status;
            wait(&status);

            //Affichage de l'image      
            imshow("CLIENT", imageFrame);

            //Les différentes commandes de l'utilisateur en fonction des touches du clavier
            char key = waitKey(30);
            if(key == (char)27){
                message = ELE4205_QUIT;
                send(clientSocket, &message, sizeof(uint32_t),0);
                cout << "Fermeture du programme" << endl;
                break;
            
            }
            else{
                message = message | ELE4205_OK;
                switch(key){

                case (char)87:
                    message = message | FORMAT_JPEG;
                    cout<<"Changement du format en JPEG" << endl;
                    break;
                
                case (char)81 :
                    message = message | FORMAT_PNG;
                    cout<<"Changement du format en PNG" << endl;
                    break;

                case (char)49 :
                    message = message|RESOLUTION_1;
                    cout<<"Changement de la resolution a 160 x 120" << endl;
                    break;

                case (char)50 :
                    message = message|RESOLUTION_2;
                    cout<<"Changement de la resolution a 352 x 288" << endl;
                    break;

                case (char)51 :
                    message = message|RESOLUTION_3;
                    cout<<"Changement de la resolution a 960 x 720" << endl;
                    break;

                case (char)52 :
                    message = message|RESOLUTION_4;
                    cout<<"Changement de la resolution a 1280 x 720" << endl;
                    break;

                case (char)114 :
                    modeChoice = 1;
                    cout<<"Vous avez choisi le mode reconnaissance !"<<endl;
                    break;
                    
                case (char)116 :
                    modeChoice = 2;
                    cout<<"Vous avez choisi le mode apprentissage !"<<endl;
                    imageTraining = 1;
                    trainingVector.clear();
                    break;

                default :
                    break;
                }
                //Envoie du message au serveur
                send(clientSocket, &message, sizeof(uint32_t),0);
            }
        }
    }
close(clientSocket);
}









