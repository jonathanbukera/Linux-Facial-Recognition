#include "../include/serverBuzzer.h"

BuzzerServer::BuzzerServer(){}

void BuzzerServer::connection(){
   int yes =1;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket == -1){
        cout << "ERREUR FATALE DANS LA CREATION DU SOCKET";
        return;
    }
    if(setsockopt(serverSocket, SOL_SOCKET,SO_REUSEADDR, &yes, sizeof(yes))<0){
        perror("setsockopt SO_REUSADDR failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    addrLen = sizeof(address);
    clientAddrLen = sizeof(clientAddress);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT2);

    int bindResult = bind(serverSocket, (struct sockaddr*)&address, addrLen);
    int listenResult = listen(serverSocket,1); // 1 accepted connection

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

    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddrLen);
    if(clientSocket<0){
        cout<<"ERREUR FATALE POUR LE ACCEPT";
        return;
    }
    if(setsockopt(clientSocket, SOL_SOCKET,SO_REUSEADDR, &yes, sizeof(yes))<0){
        perror("setsockopt SO_REUSADDR failed");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }
    
}


void BuzzerServer::endConnection(){
    close(serverSocket);
    close(clientSocket);
}

uint32_t BuzzerServer::receiveMessage(){
    uint32_t messageBuzzer;
    ssize_t messageBytes = recv(clientSocket, &messageBuzzer, sizeof(uint32_t), 0);
    if(messageBytes != sizeof(uint32_t))
    {
        cout<<"mauvaise taille de message "<< messageBytes <<endl;

    }

    if(messageBuzzer == 1){
        cout<<"message recut"<<endl;
    }
    return messageBuzzer;
        
}

void BuzzerServer::writeFiles(string path, uint32_t pinValue) const{
	ofstream fichierEcriture;
    
	fichierEcriture.open(path.c_str());
	if(fichierEcriture.is_open()){
		fichierEcriture << pinValue << endl;
		fichierEcriture.close();
	}
	else{
		cout << "Impossible d'ouvrir le fichier ! " << endl;
	}

}
void BuzzerServer::activateBuzzer(){
        uint32_t message = receiveMessage();


        if(message == KNOWN){
            //Ecriture des valeurs de frequences, d'enable et de duty cycle.
            writeFiles("/sys/devices/pwm-ctrl.42/freq0", 440);
            writeFiles("/sys/devices/pwm-ctrl.42/enable0",1);
            writeFiles("/sys/devices/pwm-ctrl.42/duty0",512);
            usleep(3000000);
            writeFiles("/sys/devices/pwm-ctrl.42/enable0",0);

        }
        else if(message == UNKNOWN){
            for(int i = 0; i<3; i++){
                writeFiles("/sys/devices/pwm-ctrl.42/freq0", 660);
                writeFiles("/sys/devices/pwm-ctrl.42/enable0",1);
                writeFiles("/sys/devices/pwm-ctrl.42/duty0",512);
                usleep(1000000);
                writeFiles("/sys/devices/pwm-ctrl.42/enable0",0);
                usleep(500000);
           
            }

        }
    
        else{
            return;

        }

}

