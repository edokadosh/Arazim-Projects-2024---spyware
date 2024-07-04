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

#include "../IncludeCPP/Status.h"
#include "command.h"
#include "responce.h"
#include "../IncludeCPP/globalDefines.h"

#include "Connection.h"

#pragma once

class PipeConnection : public Connection {


public:
    PipeConnection(int pIn, int pOut, bool closeIn, bool closeOut);
    
    static Status createPipeConnection(std::shared_ptr<PipeConnection>& pipeConn, int p[2]);

};