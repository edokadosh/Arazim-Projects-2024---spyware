#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>

#include "../IncludeCPP/Status.h"
#include "command.h"
#include "responce.h"
#include "../IncludeCPP/globalDefines.h"

#ifndef CONNECTION_H
#define CONNECTION_H


class Connection {
    
private:


public:
    int socket_;
    struct sockaddr_in address_;
    bool isOpen;

    Connection(int socket, struct sockaddr_in address);

    Connection();

    ~Connection();

    bool sendString(const std::string& str);

    bool sendResponceStruct(const responce res);

    bool sendResponce(uint32_t status, const std::string&);

    bool sendData(uint32_t size, void* buffer);

    bool recvCommand(command& cmd);

    bool recvData(uint32_t size, void* data);

    void closeSocket();

};

#endif