#include"../include/serverBuzzer.h"


using namespace std;



int main(){
    BuzzerServer myBuzzer;
    myBuzzer.connection();
    myBuzzer.activateBuzzer();
    myBuzzer.endConnection();
    return 0;
}
