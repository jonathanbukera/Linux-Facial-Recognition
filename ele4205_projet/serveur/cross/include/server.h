#ifndef SERVER_H
#define SERVER_H


#include "../../../commun/commun.h"





using namespace std;
using namespace cv;


/**
\class Server
Server est la classe qui permet la communication entre le serveur et le client pour recevoir les messages du client et d'en envoyer
**/
class Server {
    private :
        /**
        \var serverFd
        \brief socket du serveur
        **/
        int serverFd;
        /**
        \var clientFd
        \brief socket du client
        **/
        int clientFd;
        /**
        \var address
        \brief adresse du socket du serveur
        **/
        struct sockaddr_in address;
        /**
        \var clientAddress
        \brief adresse du socket du client
        **/
        struct sockaddr_in clientAddress;
        /**
        \var addrLen
        \brief longueur de l'adresse du serveur
        **/
        socklen_t addrLen;
        /**
        \var clientAddrLen
        \brief longueur de l'adresse du client
        **/
        socklen_t clientAddrLen;
         /**
        \var serverMessage
        \brief message envoye par le serveur au client
        **/
        uint32_t serverMessage;
        /**
        \var clientMessage
        \brief message recu par le serveur de la part client
        **/
        uint32_t clientMessage;
        /**
        \var buffer
        \brief buffer du message recu du client
        **/
        vector<uchar> buffer;

    public:
        /**
        \fn Server()
        \brief Constructeur de la classe Server
        **/
        Server();
        /**
        \fn connection() 
        \brief Etablit la connection entre le serveur et le client
        **/
        void connection();
        /**
        \fn lightAndButton(vector<int> val) 
        \brief Traite l'intensite de la lumiere et la valeur du boutton
        \param pinValues
        vecteur contenant les valeurs de l'ADC et du boutton
        **/
        void lightAndButton(vector<int> pinValues);
        /**
        \fn listenAndSendServer() 
        \brief Recoit les messages du clients et envoie des messages au client
        **/
        void listenAndSendServer();
};

/**
\fn captureVideo(int res1, int res2)
\brief Prends une video avec une resolution choisie par l'utilisateur
\param res1, res2
resolutions choisies par l'utilisateur
**/
void captureVideo(int res1, int res2);
/**
\fn buttonSetup()
\brief configure l'entree 0/1 du GPIO 228 (boutton)
**/
void buttonSetup();
/**
\fn readPinsFiles()
\brief retourne sous forme de vecteur de int les valeurs des fichiers de l'ADC et du boutton
**/
vector<int> readPinsFiles();




#endif