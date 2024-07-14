#include "Connection.h"

Connection::Connection(int fdInput, int fdOutput, bool needCloseInput, bool needCloseOutput)
    : fdIn(fdInput), fdOut(fdOutput), needCloseIn(needCloseInput), needCloseOut(false)
{
    if (fdIn != fdOut) {
        needCloseOut = needCloseOutput;
    }
    isSocket = false;
}

// Connection::Connection() {}

Connection::~Connection() {
    closeConn();
}

void Connection::closeConn() {
    if(needCloseIn)
        close(fdIn);
    
    if(needCloseOut)
        close(fdOut);
    needCloseIn = false;
    needCloseOut = false;    
}



int Connection::doSend(const void* buf, size_t size, int flags) {
    if (isSocket)
    {
        return ::send(fdOut, buf, size, flags);
    }
    return write(fdOut, buf, size);
}

int Connection::doSend(const void* buf, size_t size) {
    return write(fdOut, buf, size);
}

int Connection::doRecv(void* buf, size_t size, int flags) {
    std::cerr << "Connection DORECV" << std::endl;
    if (isSocket)
    {
        return ::recv(fdOut, buf, size, flags);
    }
    return read(fdOut, buf, size);
}

int Connection::doRecv(void* buf, size_t size) {
    return read(fdOut, buf, size);
}



int Connection::sendData(uint32_t size, void* buffer, int flags) {
    int bytesSent = doSend(buffer, size, flags);
    if (bytesSent == -1) {
        std::cerr << "Error sending data" << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return bytesSent;
    }
    return bytesSent;
}

int Connection::sendData(uint32_t size, void* buffer) {
    return sendData(size, buffer, 0);
}

bool Connection::sendString(const std::string& str, int flags, bool sendLegnth) {
    if (sendLegnth){
        uint32_t strLen = str.size();
        if (doSend(&strLen, sizeof(strLen), flags | MSG_MORE) == -1) {
            std::cerr << "Error sending string length" << std::endl;
            std::cerr << "Error: " << strerror(errno) << std::endl;
            return false;
        }
    }
    
    int bytesSent = doSend(str.c_str(), str.length(), flags);
    if (bytesSent == -1) {
        std::cerr << "Error sending string" << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return false;
    }
    return true;
}
bool Connection::sendString(const std::string& str, bool sendLegnth) {
    return sendString(str, 0, sendLegnth);
}

bool Connection::sendResponceStruct(const responce res, int flags) {
    int bytesSent = doSend(&res, sizeof(res), flags);
    if (bytesSent == -1) {
        std::cerr << "Error sending responce struct" << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return false;
    }
    return true;
}

bool Connection::sendResponceStruct(const responce res) {
    return sendResponceStruct(res, 0);
}

bool Connection::sendResponce(uint32_t status, const std::string& msg, int flags) {
    uint32_t dataLen = msg.length();

    if(!sendResponceStruct((responce){.dataLen = dataLen, .status = status}, flags)) {
        return false;
    }
    if (!sendString(msg, false), flags) {
        std::cerr << "Error sending responce msg" << std::endl;
        return false;
    }
    return true;
}

bool Connection::sendResponce(uint32_t status, const std::string& msg) {
    return sendResponce(status, msg, 0);
}

bool Connection::recvCommand(command& cmd, int flags)
{
    if (recvData(sizeof(cmd), (char*)&cmd, flags) != sizeof(cmd)) {
        std::cerr << "Receive command failed" << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return false;
    }

    return true;
}

bool Connection::recvCommand(command& cmd) {
    return recvCommand(cmd, 0);
}

int Connection::recvData(uint32_t size, char* buffer, int flags)
{
    int recvied = 0;
    uint32_t recviedTotal = 0;
    while (recviedTotal < size)
    {
        if ((recvied = doRecv(buffer, size - recviedTotal, flags)) == -1) {
            std::cerr << "Receive data failed after recving " << recviedTotal << " bytes" << std::endl;
            std::cerr << "Error: " << strerror(errno) << std::endl;
            return -1;
        }
        recviedTotal += recvied;
    }
    return recviedTotal;
}

int Connection::recvData(uint32_t size, char* buffer) {
    return recvData(size, buffer, 0);
}


bool Connection::sendFile(std::string filePath, int flags)
{
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return false;
    }
    const std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    uint32_t fileSize = buffer.size();
    if ((doSend(&fileSize, sizeof(fileSize), MSG_MORE | flags)) == -1) {
        std::cerr << "Error msg length" << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl;
    }
    file.close();

    if ((doSend(buffer.data(), fileSize, flags)) == -1) {
        std::cerr << "Error msg length" << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl;
    }

    return true;
}

bool Connection::sendFile(std::string filePath) {
    return sendFile(filePath, 0);
}




