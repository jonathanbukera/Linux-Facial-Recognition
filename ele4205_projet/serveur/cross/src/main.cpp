#include "../include/server.h"


using namespace std;
using namespace cv;





int main()
{   
    Server myServer;
    myServer.listenAndSendServer();

    return 0;
}