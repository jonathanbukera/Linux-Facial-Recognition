#ifndef COMMUN_H
#define COMMUN_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <time.h>
#include <map>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <sys/wait.h>
#include <cstdlib>
#include <cerrno>
#include <dirent.h>
#include <sstream>
#include<vector>

/** 
\var PORT
\brief Le port de la communication entre le serveur principal et le client 
 **/
#define PORT 4099
/** 
\var PORT2
\brief Le port de la communication entre le serveur buzzer et le client 
 **/
#define PORT2 4100
/** 
\var ELE4205_OK
\brief Le message envoyé par le client pour continuer l'image
 **/
#define ELE4205_OK 0b00000001
/** 
\var ELE4205_QUIT
\brief Le message envoyé par le client pour arreter l'image
 **/
#define ELE4205_QUIT 0b00000000
/** 
\var FORMAT_PNG
\brief Le message envoyé par le client pour changer le format de l'image en PNG
 **/
#define FORMAT_PNG 0b00000101
/** 
\var FORMAT_JPEG
\brief Le message envoyé par le client pour changer le format de l'image en JPEG
 **/
#define FORMAT_JPEG 0b00000011
/** 
\var RESOLUTION_1
\brief Le message envoyé par le client pour modifer la résolution de l'image a 160 x 120
 **/
#define RESOLUTION_1 0b00001001
/** 
\var RESOLUTION_2
\brief Le message envoyé par le client pour modifer la résolution de l'image a 352 x 288
 **/
#define RESOLUTION_2 0b00010001
/** 
\var RESOLUTION_3
\brief Le message envoyé par le client pour modifer la résolution de l'image a 960 x 720
 **/
#define RESOLUTION_3 0b00011001
/** 
\var RESOLUTION_4
\brief Le message envoyé par le client pour modifer la résolution de l'image a 1280 x 720
 **/
#define RESOLUTION_4 0b00100001
/** 
\var READY
\brief Le message envoyé par le serveur pour signaler que la lumiere est présente sur l'ADC 
 **/
#define READY 0b00000001
 /** 
\var IDOWN
\brief Le message envoyé par le serveur pour signaler que la lumiere n'est pas présente sur l'ADC 
 **/
#define IDOWN 0b00000000 
/** 
\var PUSH
\brief Le message envoyé par le serveur pour signaler que le boutton a été pesé
 **/
#define PUSHB 0b00000011
/** 
\var KNOWN
\brief Le message envoyé par le client précisant que la personne a été reconnu
 **/
#define KNOWN 0b00000001
/** 
\var UNKNOWN
\brief Le message envoyé par le client précisant que la personne n'a pas été reconnu
 **/
#define UNKNOWN 0b00000010
/** 
\var LEARN
\brief Le message envoyé par le client précisant que l'utilisateur est en mode apprentissage
 **/
#define LEARN 0b00000100
/** 
\var DEFAULT
\brief Le message envoyé par le client précisant que la personne a choici aucun des deux modes (apprentissage ou reconnaissance)
 **/
#define DEFAULT 0b00000000


#endif