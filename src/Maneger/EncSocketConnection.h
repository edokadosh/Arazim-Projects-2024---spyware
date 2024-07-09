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
#include "SocketConnection.h"

#pragma once

class EncSocketConnection : public SocketConnection {

    int doSend(const void* buf, size_t size, int flags) override;
    int doRecv(void* buf, size_t size, int flags) override;


};