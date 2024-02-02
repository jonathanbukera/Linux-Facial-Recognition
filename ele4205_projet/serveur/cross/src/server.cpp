#include "../include/server.h"



using namespace std;
using namespace cv;



Server::Server(){}


void Server::connection(){
    int yes =1;

    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverFd == -1){
        cout << "ERREUR FATALE DANS LA CREATION DU SOCKET";
        return;
    }
    if(setsockopt(serverFd, SOL_SOCKET,SO_REUSEADDR, &yes, sizeof(yes))<0){
        perror("setsockopt SO_REUSADDR failed");
        close(serverFd);
        exit(EXIT_FAILURE);
    }

    addrLen = sizeof(address);
    clientAddrLen = sizeof(clientAddress);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    int bindResult = bind(serverFd, (struct sockaddr*)&address, addrLen);
    int listenResult = listen(serverFd,1); // 1 accepted connection

    if(bindResult < 0 || listenResult  ){
        cout << "ERREUR FATALE DANS LE BIND";
        return;
    }
    if(listenResult < 0){
             cout << "ERREUR FATALE POUR LE LISTEN";
             return;
        }
    if(listenResult >= 0){
        cout<<"attente d'une connection \n";
    }

    clientFd = accept(serverFd, (struct sockaddr*)&clientAddress, &clientAddrLen);
    if(clientFd<0){
        cout<<"ERREUR FATALE POUR LE ACCEPT";
        return;
    }
    if(setsockopt(clientFd, SOL_SOCKET,SO_REUSEADDR, &yes, sizeof(yes))<0){
        perror("setsockopt SO_REUSADDR failed");
        close(clientFd);
        exit(EXIT_FAILURE);
    }

}

vector<int> readPinsFiles(){
    string pathGPIO = "/sys/class/gpio/gpio228/value";
    string pathADC = "/sys/class/saradc/ch0";
    vector<int> pinValues, empty;
    int GPIO;
    int ADC;

    ifstream fichierLec(pathGPIO.c_str());
    if(fichierLec.is_open()){
        fichierLec >> GPIO;
        pinValues.push_back(GPIO);
    }
    else{
        cout<<"ERREUR DANS L'OUVERTURE DU FICHIER"<<endl;
        return empty;
    }
    cout<<GPIO<<endl;
    ifstream ADCFichierLec(pathADC.c_str());
    if(ADCFichierLec.is_open()){
        ADCFichierLec >> ADC;
        pinValues.push_back(ADC);
    }
    else{
        cout<<"ERREUR DANS L'OUVERTURE DU FICHIER"<<endl;
        return empty;
    }
    cout<<ADC<<endl;
    return pinValues;
}


void buttonSetup(){
    ofstream fichierEcriture1;
    ofstream fichierEcriture2;
    string path1 = "/sys/class/gpio/export";
    string path2 = "/sys/class/gpio/gpio228/direction";

	fichierEcriture1.open(path1.c_str());
	if(fichierEcriture1.is_open()){
		fichierEcriture1 << 228 << endl;
		fichierEcriture1.close();
	}
	else{
		cout << "Impossible d'ouvrir le fichier 1 ! " << endl;
	}
    fichierEcriture2.open(path2.c_str());
	if(fichierEcriture2.is_open()){
		fichierEcriture2 << "in" << endl;
		fichierEcriture2.close();
	}
	else{
		cout << "Impossible d'ouvrir le fichier 2 ! " << endl;
	}
}


void Server::lightAndButton(vector<int> pinValues){
    int pid = -1;
    bool lumiere = false;
    while(lumiere == false){
        
        if(pinValues[1] < 800){
            lumiere = true;
            
            if(pinValues[0] == 0){
                // Une fois le boutton pese, on envoie un message au client, et on fait un fork pour executer le programme du buzzer
                serverMessage = PUSHB;
                send(clientFd, &serverMessage, sizeof(uint32_t),0);
                pid = fork();
                if(pid == 0){ 
                    system("/home/root/serverBuzzer");
                    exit(0);
                }
                }
            else{
                // Il y a de la lumiere donc l'image est disponible et on envoie le message au client
                serverMessage = READY;
                send(clientFd, &serverMessage, sizeof(uint32_t),0);
            }
        }
        else{
            // On envoie au client le message qu'il n'y a pas de lumiere, donc pas d'image
            serverMessage = IDOWN;
            send(clientFd, &serverMessage, sizeof(uint32_t),0);
            pinValues.clear();
            pinValues = readPinsFiles();
        }
        usleep(100000);
    }
}


void Server::listenAndSendServer(){

    // Longueur du message recu par le client    
    size_t lengthMsg = 1;

    // Les caracteristiques de l'image
    Mat image;
    int res1 = 352;
    int res2 = 288;
    double fps = 29.4314;
    string format = ".png";
    
    //On commence la connection avec le client et on initialise les fichiers de l'ADC et du boutton
    connection();
    buttonSetup();
    vector<int> pinValues = readPinsFiles();
    lightAndButton(pinValues);

    // On active la camera
    VideoCapture capture(0);
    capture.set(CV_CAP_PROP_FRAME_WIDTH,res1);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT,res2);
    //capture.set(CV_CAP_PROP_FPS, fps);
    if(!capture.isOpened()){
        cout<<"ERREUR DANS L'OUVERTURE DE LA CAMERA"<<endl;
        close(clientFd);
        close(serverFd);
        return;
    }
    //Communication entre le serveur et le client qui permet de modifier les parametres de l'image
    while(1){

        pinValues.clear();
        capture >> image;
        buffer.clear();
        clientMessage = 0;

        // Encodage de l'image
        imencode(format,image,buffer);
        uint bufferSize = buffer.size();
        send(clientFd,&bufferSize, sizeof(uint),0);

        // Envoie un byte a la fois de l'image au client
        for(int i = 0; i<buffer.size(); i++){
            send(clientFd, &buffer[i], lengthMsg, 0);
        }
        
        // Reception du message du client
        ssize_t messageBytes = recv(clientFd, &clientMessage, sizeof(uint32_t), 0);
        
            if(messageBytes != sizeof(uint32_t))
            {
                break;
            }
            // On ferme le programme
            if (clientMessage == ELE4205_QUIT)
            {
                cout << "COMMUNICATION ENDED \n";
                break;
            }

            else {
                // Modification du format de l'image ou de la resolution
                switch(clientMessage){

                    case FORMAT_PNG :
                    format = ".png";
                        break;

                    case FORMAT_JPEG :
                        format = ".jpeg";
                        break;

                    case RESOLUTION_1 :
                        res1 = 160;
                        res2 = 120;
                        fps = 29.7309;
                        capture.set(CV_CAP_PROP_FRAME_WIDTH,res1);
                        capture.set(CV_CAP_PROP_FRAME_HEIGHT,res2);
                        //capture.set(CV_CAP_PROP_FPS, fps);
                        break;

                    case RESOLUTION_2 :
                        res1 = 352;
                        res2 = 288;
                        fps = 29.4314;
                        capture.set(CV_CAP_PROP_FRAME_WIDTH,res1);
                        capture.set(CV_CAP_PROP_FRAME_HEIGHT,res2);
                        //capture.set(CV_CAP_PROP_FPS, fps);
                        break;

                    case RESOLUTION_3 :
                        res1 = 960;
                        res2 = 720;
                        fps = 23.14;
                        capture.set(CV_CAP_PROP_FRAME_WIDTH,res1);
                        capture.set(CV_CAP_PROP_FRAME_HEIGHT,res2);
                        //capture.set(CV_CAP_PROP_FPS, fps);
                        break;

                    case RESOLUTION_4 :
                        res1 = 1280;
                        res2 = 720;
                        fps = 18.4356;
                        capture.set(CV_CAP_PROP_FRAME_WIDTH,res1);
                        capture.set(CV_CAP_PROP_FRAME_HEIGHT,res2);
                        //capture.set(CV_CAP_PROP_FPS, fps);
                        break;
                    
                    default:
                        format = format;
                        cout<<"FORMAT : "<< format <<endl;
                }
            }
            cout<<clientMessage<<endl;
            pinValues = readPinsFiles();
            lightAndButton(pinValues);
    }
    close(clientFd);
    close(serverFd);
    }



//livrable 1
void captureVideo(int res1, int res2){
    double fps = 0;

    map<int,double> resolutionFrame;
    resolutionFrame[144] =  29.6395;
    resolutionFrame[120] = 29.7309;
    resolutionFrame[176] = 29.5351;
    resolutionFrame[240] = 29.4291;
    resolutionFrame[288] = 29.4314;
    resolutionFrame[240] = 29.5345;
    resolutionFrame[600] = 29.002;
    resolutionFrame[480] = 29.079;
    resolutionFrame[544] = 28.9229;
    resolutionFrame[720] = 23.14;
    resolutionFrame[656] = 20.2997;
    resolutionFrame[720] = 18.4356;
    resolutionFrame[960] = 13.8853;

    if(resolutionFrame.find(res2) != resolutionFrame.end()){
         fps = resolutionFrame[res2];
    }
    else{
        cout<<"erreur ";
        return;
    }

    VideoCapture capture(0);
    if(!capture.isOpened()){
	    cout << "Failed to connect to the camera." << endl;
    }
    capture.set(CV_CAP_PROP_FRAME_WIDTH,res1);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT,res2);
    capture.set(CV_CAP_PROP_FPS, fps);
    string videoName = "capture-liv1.avi";

    VideoWriter videoWrite(videoName, CV_FOURCC('M', 'J', 'P', 'G'),fps, Size(res1, res2));

    if(!videoWrite.isOpened()){
	    cout << "Failed to open ouput file." << endl;
    }

    double duree = 5.0;
    double startTime = static_cast<double>(getTickCount())/getTickFrequency();
    double currentTime = 0.0;
    int count = 0;

    while(currentTime<=duree){
        
        Mat frame;
        capture >> frame;
        if(frame.empty()){
            cout<<"Erreur";
            break;
        }
       
            videoWrite.write(frame);
            currentTime = (static_cast<double>(getTickCount()))/getTickFrequency()-startTime;

    }
    capture.release();
    videoWrite.release();
}





