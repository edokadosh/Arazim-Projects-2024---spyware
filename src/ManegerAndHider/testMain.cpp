
#include <iostream>

#include "command.h"
#include "Connection.h"
#include "Listener.h"

#define PORT (65432)

int main(int argc, char* argv[])
{
    Listener listener = Listener(PORT);

    while (true)
    {
        Connection conn;
        command cmd;

        if (listener.acceptConnection(conn) == -1) {
            continue;
        }
        
        if (!conn.recvCommand(cmd)) {
            continue;
        }
        std::cout << "cmd.dataLen: " << cmd.dataLen << std::endl;
        std::cout << "cmd.funcode: " << cmd.fncode << std::endl;
        std::cout << "cmd.strParam: " << cmd.strParam << std::endl;
    }
}