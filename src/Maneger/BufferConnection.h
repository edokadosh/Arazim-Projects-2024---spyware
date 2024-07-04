#include <cerrno>
#include <vector>
#include <memory>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "Connection.h"
#include "../IncludeCPP/Status.h"
#include "command.h"
#include "responce.h"
#include "../IncludeCPP/globalDefines.h"

#pragma once


#pragma once

class BufferConnection : public Connection {
protected:
    uint32_t ctr = 0;
    char* buffer;
    bool isIn;

    int doSend(const void* buf, size_t size) override;
    int doRecv(void* buf, size_t size) override;


public:
    BufferConnection(char* buf, bool isInput);
};