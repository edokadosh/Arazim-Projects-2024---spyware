#pragma once

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


class Connection {
    
protected:
    int fdIn;
    int fdOut;
    bool needCloseIn;
    bool needCloseOut;

    virtual int doSend(const void* buf, size_t size, int flags);
    virtual int doSend(const void* buf, size_t size);
    virtual int doRecv(void* buf, size_t size, int flags);
    virtual int doRecv(void* buf, size_t size);
    bool isSocket;

public:

    Connection(int fdInput, int fdOutput, bool needCloseInput, bool needCloseOutput);

    // Connection(Connection&& other) noexcept;
    // Connection& operator=(Connection&& other) noexcept;

    ~Connection();


    void closeConn(void);

    bool sendString(const std::string& str, bool sendLegnth);
    bool sendString(const std::string& str, int flags, bool sendLegnth);

    bool sendResponceStruct(const responce res);
    bool sendResponceStruct(const responce res, int flags);

    bool sendResponce(uint32_t status, const std::string& ansString);
    bool sendResponce(uint32_t status, const std::string& ansString, int flags);

    int sendData(uint32_t size, void* buffer);
    int sendData(uint32_t size, void* buffer, int flags);

    bool recvCommand(command& cmd);
    bool recvCommand(command& cmd, int flags);

    int recvData(uint32_t size, char data[]);
    int recvData(uint32_t size, char data[], int flags);

    bool sendFile(std::string filePath);
    bool sendFile(std::string filePath, int flags);

    int flushInput(void);

friend class HiderManeger;
};
