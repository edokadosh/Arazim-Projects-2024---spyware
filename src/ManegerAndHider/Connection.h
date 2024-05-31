#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>

#include "Status.h"
#include "command.h"
#include "responce.h"

#ifndef CONNECTION_H
#define CONNECTION_H

#define CHUNK_SIZE (1024)

class Connection {
    
private:
    int socket_;
    struct sockaddr_in address_;


public:

    Connection(int socket, struct sockaddr_in address);

    Connection();

    ~Connection();

    bool sendResponce(const responce res);

    bool sendData(const std::string& msg);

    bool recvCommand(command& cmd);

    bool recvData(char data[CHUNK_SIZE]);

};

#endif