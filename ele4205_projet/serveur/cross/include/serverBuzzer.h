#ifndef SERVERBUZZER_H
#define SERVERBUZZER_H

#include "../../../commun/commun.h"


using namespace std;

/**
\class BuzzerServer
BuzzerServer est la classe qui permet de connecter un deuxieme serveur au client afin d'activer le buzzer et emettre un son selon la face reconnu ou non
**/
class BuzzerServer{

    private :
    /**
    \var clientSocket
    \brief Socket du client
    **/
    int clientSocket;
    /**
    \var serverSocket
    \brief Socket du serveur
    **/
    int serverSocket;
    /**
    \var address
    \brief Adresse du serveur
    **/
    struct sockaddr_in address;
    /**
    \var clientAddress
    \brief Adresse du client
    **/
    struct sockaddr_in clientAddress;
    /**
    \var addrLen
    \brief Longueur de l'adresse du serveur
    **/
    socklen_t addrLen;
     /**
    \var clientAddrLen
    \brief Longueur de l'adresse du client
    **/
    socklen_t clientAddrLen;
    

    public :
    /**
    \fn BuzzerServer()
    \brief Constructeur de la classe BuzzerServer
    **/
    BuzzerServer();
    /**
    \fn connection()
    \brief Debute une connection entre ce serveur et le client
    **/
    void connection();
    /**
    \fn receiveMessage()
    \brief Recoit le message du client
    **/
    uint32_t receiveMessage();
    /**
    \fn endConnection()
    \brief Ferme les sockets de la connection
    **/
    void endConnection();
    /**
    \fn writeFiles(string path, uint32_t x) const 
    \brief Ecrit les valeurs de la frequence, duty cycle et enable pour le buzzer
    \param path
    string qui inclut le path du fichier a ecrire la valeur
    \param pinValue
    uint32_t valeur a ecrire dans le fichier
    **/
    void writeFiles(string path, uint32_t pinValue) const;
     /**
    \fn activateBuzzer()
    \brief Active le son du buzzer
    **/
    void activateBuzzer();
   
};







#endif
