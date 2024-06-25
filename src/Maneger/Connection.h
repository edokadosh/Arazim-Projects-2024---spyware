#include <iostream>
#include <fstream>
#include <sys/socket.h>
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
    int fdIn;
    int fdOut;
    bool isSocket;
    bool needCloseIn;
    bool needCloseOut;

    int doSend(const void* buf, size_t size, int flags);
    int doSend(const void* buf, size_t size);
    int doRecv(void* buf, size_t size, int flags);
    int doRecv(void* buf, size_t size);

public:

    Connection(int fdInput, int fdOutput, bool needCloseInput, bool needCloseOutput, bool isSocket);

    // Connection(Connection&& other) noexcept;
    // Connection& operator=(Connection&& other) noexcept;

    ~Connection();

    void closeConn(void);

    bool sendString(const std::string& str);
    bool sendString(const std::string& str, int flags);

    bool sendResponceStruct(const responce res);
    bool sendResponceStruct(const responce res, int flags);

    bool sendResponce(uint32_t status, const std::string& ansString);
    bool sendResponce(uint32_t status, const std::string& ansString, int flags);

    bool sendData(uint32_t size, void* buffer);
    bool sendData(uint32_t size, void* buffer, int flags);

    bool recvCommand(command& cmd);
    bool recvCommand(command& cmd, int flags);

    int recvData(uint32_t size, char data[]);
    int recvData(uint32_t size, char data[], int flags);

    bool sendFile(std::string filePath);
    bool sendFile(std::string filePath, int flags);


friend class HiderManeger;
};

#endif