#include "Listener.h"

const int BACKLOG = 5;


Listener::Listener(int port)
{
    
    if ((listeningSocket_ = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        std::cerr << "Error creating listening socket" << std::endl;
        return;
    }

    address_.sin_family = AF_INET;
    address_.sin_addr.s_addr = INADDR_ANY;
    address_.sin_port = htons(port);

    if (bind(listeningSocket_, (struct sockaddr *)&address_, sizeof(address_)) == -1) {
        std::cerr << "Error binding listening socket" << std::endl;
        return;
    }

    if (listen(listeningSocket_, BACKLOG) == -1) {
        std::cerr << "Error listening" << std::endl;
        return;
    }
}


Listener::~Listener() {
    close(listeningSocket_);
}


int Listener::acceptConnection(Connection& conn) {
        int homeSocket;
        struct sockaddr_in homeAddr;
        socklen_t homeAddrLen = sizeof(homeAddr);

        // Accept connection
        homeSocket = accept(listeningSocket_, (struct sockaddr *)&homeAddr, &homeAddrLen);
        if (homeSocket == -1) {
            std::cerr << "Error accepting connection" << std::endl;
            return -1;
        }
        conn = Connection(homeSocket, homeSocket, true, true);
        return homeSocket;
    }
