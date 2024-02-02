#ifndef CLIENT_H
#define CLIENT_H


#include<iostream>
#include "../../commun/commun.h"



using namespace std;
using namespace cv;


/**
\class Client
Client est la classe qui connecte aux deux serveurs pour la réception et envoie des messages, et offre plusieurs commandes d'utilisation du programme au client 
**/
class Client{

    private:
        /**
        \var serverAddress
        \brief Adresse du serveur
        **/
        struct sockaddr_in serverAddress;
        /**
        \var serverAddress
        \brief Adresse du client
        **/
        struct sockaddr_in clientAddress;
        /**
        \var clientSocket
        \brief Socket du client
        **/
        int clientSocket;
        /**
        \var addrLen
        \brief Longueur de l'addresse du serveur
        **/
        socklen_t addrLen;
        /**
        \var clientAddrLen
        \brief Longueur de l'addresse du client
        **/
        socklen_t clientAddrLen;
        /**
        \var message
        \brief Message envoyé par le client au serveur
        **/
        uint32_t message;
        /**
        \var serverMessage
        \brief Message recu de la part du serveur
        **/
        uint32_t serverMessage;
        /**
        \var buzzerMessage
        \brief Message envoyé par le client au serveur buzzer
        **/
        uint32_t buzzerMessage;
        /**
        \var numberOfFaceRecognize
        \brief Nombre de visages reconnu par la reconnaissance
        **/
        int numberOfFaceRecognize;
        /**
        \var tempMode
        \brief Connaitre le mode choisi par l'utilisateur dans le child process
        **/
        int tempMode;
        /**
        \var name
        \brief Nom des visages appris
        **/
        string name;
        /**
        \var faceImg
        \brief Image de l'utilisateur en mode apprentissage avec le visage détecté
        **/
        Mat faceImg;
         /**
        \var label
        \brief Vecteur contenant les ID des visages
        **/
        vector<int> label;
        /**
        \var imageData
        \brief Octet de l'image transmis par le serveur
        **/
        uchar imageData;
        /**
        \var imageFrame
        \brief Image decode apres sa transmission par le serveur
        **/
        Mat imageFrame;
        /**
        \var trainingVector
        \brief Vecteur contenant les images utilisées par le mode apprentissage
        **/
        vector<Mat> trainingVector;
        /**
        \var buffer
        \brief Vecteur contenant les tampons des octets de l'image
        **/
        vector<uchar> buffer;
        /**
        \var buffer
        \brief Taille du tampon des octets
        **/
        uint bufferSize;

    public:
        /**
        \fn Client()
        \brief Constructeur de la classe Client
        **/
        Client();
        /**
        \fn connection(int port)
        \brief Connection pour chacun des deux serveurs
        \param port
        Numéro du port pour la connexion
        **/
        void connection(int port);
        /**
        \fn sendAndReceiveMessage()
        \brief Communication entre le serveur et client, permet de lui envoyer les commandes de l'utilisateur et de faire l'apprentissage et la reconnaissance dans le child process
        **/
        void sendAndReceiveMessage();
        /**
        \fn menu()
        \brief Affiche dans le terminal les commandes de fonctions possibles pour l'utilisateur
        **/
        void menu();

};




#endif