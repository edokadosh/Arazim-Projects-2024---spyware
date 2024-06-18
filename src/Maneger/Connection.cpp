#include <cerrno>
#include <vector>
#include "Connection.h"
#include "responce.h"

Connection::Connection(int fdInput, int fdOutput, bool needCloseInput, bool needCloseOutput, bool isSock)
{
    fdIn = fdInput;
    fdOut = fdOutput;
    isSocket = isSock;

    needCloseIn = needCloseInput;
    if (fdIn != fdOut) {
        needCloseOut = needCloseOutput;
    }
}

Connection::Connection() {}

Connection::~Connection() {
    // std::cout << "deastroy" << std::endl;
    // if (socket_ != -1) {
    //     close(socket_);
    // }
    // needCloseIn = false;
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
    if (isSocket)
    {
        return ::recv(fdOut, buf, size, flags);
    }
    return read(fdOut, buf, size);
}

int Connection::doRecv(void* buf, size_t size) {
    return read(fdOut, buf, size);
}

void Connection::closeConnection() {
    if(needCloseIn)
        close(fdIn);
    
    if(needCloseOut)
        close(fdOut);
}


bool Connection::sendData(uint32_t size, void* buffer, int flags) {
    int bytesSent = doSend(buffer, size, flags);
    if (bytesSent == -1) {
        std::cerr << "Error sending data" << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return false;
    }
    return true;
}

bool Connection::sendData(uint32_t size, void* buffer) {
    return sendData(size, buffer, 0);
}

bool Connection::sendString(const std::string& str, int flags) {
    int bytesSent = doSend(str.c_str(), str.length(), flags);
    if (bytesSent == -1) {
        std::cerr << "Error sending string" << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return false;
    }
    return true;
}
bool Connection::sendString(const std::string& str) {
    return sendString(str, 0);
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
    if (!sendString(msg), flags) {
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
    if (doRecv(&cmd, sizeof(cmd), flags) != sizeof(cmd)) {
        std::cerr << "Receive command failed" << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return false;
    }

    return true;
}

bool Connection::recvCommand(command& cmd) {
    return recvCommand(cmd, 0);
}

bool Connection::recvData(uint32_t size, char* buffer, int flags)
{
    if (doRecv(buffer, size, flags) == -1) {
        std::cerr << "Receive data failed" << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return false;
    }
    return true;
}

bool Connection::recvData(uint32_t size, char* buffer) {
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


