#include "../include/faces.h"

using namespace std;
using namespace cv;


vector<Rect> detectVisagesReconnaissance(const string imgPath, int index){
    ostringstream oss;
    oss << index;
    CascadeClassifier face_cascade;
    //fichier utilisé pour la détection de visages
    if(!face_cascade.load("../haarcascade_frontalface_default.xml"))
    {
        cout << "ERREUR DANS L'OUVERTURE DU FICHIER XML";
        vector<Rect> error;
        return error;
    }
    else 
    {
        Mat image = imread(imgPath);
        if(image.empty())
        {
            cout << "ERREUR DANS L'OUVERTURE DE L'IMAGE";
            vector<Rect> error;
            return error;
        }

        // Conversion de l'image en gray scaled et égalisation d'histogramme pour une meilleure détection de visages
        Mat gray_image;
        cvtColor(image, gray_image, COLOR_BGR2GRAY);
        equalizeHist(gray_image, gray_image);
        //imwrite(imgPath, gray_image);

        // Détection des visages dans l'image
        vector<Rect> faces;
        face_cascade.detectMultiScale(gray_image, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30));

        //Ajout des rectangles de détection autour des visages
        for(size_t i = 0; i < faces.size(); i++)
        {
            rectangle(gray_image, faces[i], Scalar(0, 0, 255), 2 , 8, 0);
        }

        //Sauvegarde de l'image avec la détection
        string grayImgPath = "../Detection/" + oss.str() + ".png";
        if(!imwrite(grayImgPath, gray_image))
        {
            cout << "ERREUR DANS LA SAUVEGARDE DE L'IMAGE";
        }
        
        oss.clear();
        oss.str("");
        return faces;

    }
}

Mat detectVisageApprentissage(Mat image){
    CascadeClassifier face_cascade;
    Mat emptyImage;
    if(!face_cascade.load("../haarcascade_frontalface_default.xml"))
    {
        cout << "ERREUR DANS L'OUVERTURE DU FICHIER XML";
        return emptyImage;
    }
    else 
    {
        Mat gray_image;
        cvtColor(image, gray_image, COLOR_BGR2GRAY);
        equalizeHist(gray_image, gray_image);
       
        vector<Rect> faces;
        face_cascade.detectMultiScale(gray_image, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30));
        if(faces.empty()){
            cout<<"AUCUN VISAGE DETECTE"<<endl;
            return emptyImage;
        }
        cout << "VISAGE DETECTE" << endl;
        rectangle(gray_image, faces[0], Scalar(0, 0, 255), 2 , 8, 0);

        Mat crop = gray_image(faces[0]);
        Mat cropResized;
        resize(crop, cropResized, Size(300, 300));

        return cropResized;

    }
}



vector<int> reconnaissance(const string imgPath, vector<Rect> faces){
    string name;
    vector<int> facesLabels;

    //Seuils d'acceptetion de la reconnaissance dépendament du nombre de visage détecté
    int seuil = 80;
    if(faces.size()>1){
            seuil = 150;
        }

    //Chargement du model de reconnaissance
     Ptr<FaceRecognizer> model =  createLBPHFaceRecognizer();
     model ->load("../DatasetFace.yml");
     Mat gray_image = imread(imgPath, CV_LOAD_IMAGE_GRAYSCALE);

     //Pour chaque visage détecté on fait une reconnaissance
     for(int i = 0; i<faces.size(); i++){
        Mat crop = gray_image(faces[i]);
        Mat faceIm;
        resize(crop, faceIm, Size(300, 300));
        int predicted_label = -1;
        double confiance = 0.0;
        model->predict(faceIm, predicted_label,confiance);

        //Si la confiance est en bas du seuil la reconnaissance du visage est inconnue
        if (confiance>seuil){
            predicted_label = -1;
        }
        name = getNameFromId(predicted_label);
 

        cout<<predicted_label<<" "<<confiance<<endl;
        cout << "Le nom de l'utilisateur reconnu est : " << name << endl;
        facesLabels.push_back(predicted_label);

     }
     return facesLabels;
}


void preTrainingModel(vector<Mat> image, string name){
    
    //Pour chaque image on rajoute un label par rapport au nom
    vector<int> labels;
    for(int i = 0; i<image.size(); i++){
        labels.push_back(getId(name));
    }
    //On entraine pour la premiere fois le modele de reconnaissance
    Ptr<FaceRecognizer> model =  createLBPHFaceRecognizer();
    model->train(image, labels);
    model->save("../DatasetFace.yml");
}


int getId(string name){
    //On associe un nom avec id
    int id = 0;
    for (int i = 0; i<name.length(); i++){
        id+=static_cast<int>(name[i]);
    }
    return id;
}

bool readFiles(string name){
    //on lit le contenu du fichier
    string nomFichier = "../NamesAndId.txt";
    ifstream inputFile(nomFichier.c_str());
    if(!inputFile.is_open()){
        cout<<"ERREUR DANS L'OUVERTURE DU FICHIER."<<endl;
    }
    //on cherche un nom a travers le fichier texte
    string line;
    while(getline(inputFile, line)){
        if(line.find(name)!=string::npos){
            inputFile.close();
            return true;
        }

    }
    inputFile.close();
    return false;
}



void namesFile(string name){
    //on écrit le nom dans le fichier texte
    int Id = getId(name);

    ofstream file2("../NamesAndId.txt", ios::app);
    if(file2.is_open()){
        file2<< "Nom: " << name << "       ID: " << Id << endl;
        file2.close();
    }
    else{
        cout<<"OUVERTURE DU FICHIER IMPOSSIBLE"<<endl;
    }

    
}

void updateModel(vector<Mat> newImages, string name){

    Ptr<FaceRecognizer> model =  createLBPHFaceRecognizer();
    model ->load("../DatasetFace.yml");
    vector<int> newLabels;
    for(int i = 0; i<newImages.size(); i++){
        newLabels.push_back(getId(name));
    }

    //On entraine lorsque le modele de reconnaissance existe déja
    model->update(newImages,newLabels);
    model ->save("../DatasetFace.yml");

}



string getNameFromId(int label){
    
    ostringstream labelSS;
    labelSS << label;
    string labelString = labelSS.str();

    ifstream file("../NamesAndId.txt");
    string line;
    string name = "UNKNOWN USER";

   //on cherche le ID dans le fichier texte pour obtenir le nom associe
   while(getline(file, line))
   {
    size_t idPos = line.find("ID: ");
    if(idPos != string::npos)
    {
        string id = line.substr(idPos + 4);
        if(id == labelString)
        {
            size_t namePos = line.find("Nom: ");
            if(namePos != string::npos)
            {
                name = line.substr(namePos + 4, idPos - namePos -6);
                break;
            }
        }
    }
   }
   
   return name;
}