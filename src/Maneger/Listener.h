#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>

#include "../IncludeCPP/Status.h"
#include "../IncludeCPP/globalDefines.h"
#include "Connection.h"

#ifndef LISTENER_H
#define LISTENER_H

class Listener {

private:
    int listeningSocket_;
    struct sockaddr_in address_;


public:
    Listener(int port);

    ~Listener();

    int acceptConnection(Connection& conn);

};


#endif