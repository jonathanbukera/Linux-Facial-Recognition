#ifndef FACES_H
#define FACES_H


#include "client.h"
#include "../../commun/commun.h"



using namespace std;
using namespace cv;


/**
\fn preTrainingModel(vector<Mat> image, string name)
\brief Premier apprentissage lorsque le fichier "yml" n'existe pas
\param image, name
Vecteur d'image pour l'apprentissage et nom du visage appris
**/
void preTrainingModel(vector<Mat> image, string name);
/**
\fn reconnaissance(const string imgPath, vector<Rect> faces)
\brie Reconnaissance d'un visage
\param imgPath, faces
Nom du fichier de l'image prit pour la reconnaissance vector de visage détecté
**/
vector<int> reconnaissance(const string imgPath, vector<Rect> faces);
/**
\fn getId(string name)
\brief Conversion du nom en ASCII pour avoir un ID
\param name
Nom de l'utilisateur en mode apprentissage
**/
int getId(string name);
/**
\fn namesFile(string name)
\brief Ecriture du nom de l'utilisateur dans le fichier texte si il n'existais pas
\param name
Nom de l'utilisateur en mode apprentissage
**/
void namesFile(string name);
/**
\fn updateModel(vector<Mat> newImages, string name)
\brief Mode apprentissage lorsque le fichier "yml" existe qui met a jour le modele existant
\param newImage, name
Vecteur de nouvelles images du visages de l'utilisateur dans le mode apprentissage et son nom
**/
void updateModel(vector<Mat> newImages, string name);
/**
\fn readFiles(string name)
\brief Recherche le nom de l'utilisateur dans le fichier text
\param name
Nom de l'utilisateur
**/
bool readFiles(string name);
/**
\fn getNameFromId(int Id)
\brief Retourne le nom de(s) l'utilisateur(s) apres la reconnaissance a partir du ID 
\param Id
ID de l'utilisateur reconnu
**/
string getNameFromId(int Id);
/**
\fn detectVisagesReconnaissance(const string imgPath, int index)
\brief Détection des visages sur une image pour le mode reconnaissance
\param imgPath, index
Nom du fichier de l'image sur laquel on fait la detection et index de l'image utilisé
**/
vector<Rect> detectVisagesReconnaissance(const string imgPath, int index);
/**
\fn Mat detectVisageApprentissage(Mat image)
\brief Detection du visage sur une image pour le mode apprentissage, le convertit en gray scaled et le crop
\param image
Image sur laquel on fait la détection du visage pour le mode apprentissage
**/
Mat detectVisageApprentissage(Mat image);


#endif