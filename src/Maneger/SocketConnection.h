#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <vector>
#include <memory>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "Connection.h"
#include "../IncludeCPP/Status.h"
#include "command.h"
#include "responce.h"
#include "../IncludeCPP/globalDefines.h"

#pragma once

class SocketConnection : public Connection {
private:
    struct addrinfo peerAddr;

public:
    SocketConnection(int fdInput, int fdOutput, bool needCloseInput, bool needCloseOutput, struct addrinfo addr);

    static int connectTCP(std::string host, int port, std::shared_ptr<SocketConnection>& conn);

    int flushInput();

};