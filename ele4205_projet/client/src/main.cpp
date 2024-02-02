#include "../include/client.h"
#include "../include/faces.h"


using namespace std;
using namespace cv;

/**
\mainpage
Ce projet est divisé en trois section, le client, le serveur et le commun.
<br><br>
Dans le répertoire client il y a trois ".cpp" (main.cpp, client.cpp et faces.cpp) et deux ".h" (client.h, faces.h).
Les fichiers ".h" incluent la déclarations de toutes les fonctions. Le fichier "client.h" inclu la classe Client et toutes les fonctions utilisées pour la connexion et la communication avec les deux serveurs.
Le fichier "faces.h" se concentre sur la déclaration des fonctions qui sont utilisées pour tout ce qui est reliée a OpenCV ainsi que l'écriture et la lecture du fichier "NamesAndId.txt" (pour la reconnaissance). 
<br>
On a par la suite les fichiers ".cpp" du répertoire client qui définit les fonctions présentes dans chacun des fichiers ".h". Le fichier "main.cpp" permet par la suite l'exécution de ces deux autres fichiers ".cpp" du programme client.
<br><br>
Dans le répertoire serveur il y a un second répertoire du nom de "cross" ou la compilation croisé est faite. Dans le répertoire "cross" il y a  quatre ".cpp" (main.cpp, main2.cpp server.cpp et serverBuzzer.cpp) et deux ".h" (server.h, serverBuzzer.h).
Le fichier "server.h" contient la classe "Server" et fait la déclaration de toutes les fonctions en lien avec la communication du premier serveur et de la prise d'image avec la caméra.
Le fichier "serverBuzzer.h" contient la classe "BuzzerServer" et fait la déclaration de toutes les fonctions en lien avec la communication avec le deuxieme serveur qui sert a activer le buzzer.
<br>
On a ensuite les fichiers "server.cpp" et "serverBuzzer.cpp" qui font la définition de leur fichier ".h". les deux "main" sevrent a éxecuter les deux éxecutables respectif du programme (serveur et serverBuzzer).
<br><br>
En dernier lieu, le répertoire commun contient le fichier "commun.h" qui sert a inclure les "header files" ou "packages" et définir les constantes utilisées par tous les autres répertoire du projet. 
**/


int main(){
    
    Client myClient;
    myClient.sendAndReceiveMessage();
    return 0;
}
