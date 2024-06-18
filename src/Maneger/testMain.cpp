
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
            std::cerr << "listener.acceptConnection(conn) failed" << std::endl;
            continue;
        }
        std::cout << "conn.socket_: " << conn.socket_ << std::endl;
        std::cout << "conn.needCloseIn: " << conn.needCloseIn << std::endl;
        
        if (!conn.recvCommand(cmd)) {
            std::cerr << "conn.recvCommand(cmd) failed" << std::endl;
            conn.~Connection();
            continue;
        }
        std::cout << "cmd.dataLen: " << cmd.dataLen << std::endl;
        std::cout << "cmd.funcode: " << cmd.fncode << std::endl;
        std::cout << "cmd.strParam: " << cmd.strParam << std::endl;

        conn.sendResponceStruct((responce){.dataLen = 65, .status = FILE_WRITE_ERROR});

        conn.~Connection();
    }
}