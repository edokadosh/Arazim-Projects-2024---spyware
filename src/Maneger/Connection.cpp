#include <cerrno>
#include <vector>
#include "Connection.h"
#include "responce.h"

Connection::Connection(int fdInput, int fdOutput, bool needCloseInput, bool needCloseOutput)
{
    fdIn = fdInput;
    fdOut = fdOutput;

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

void Connection::closeConnection() {
    if(needCloseIn)
        close(fdIn);
    
    if(needCloseOut)
        close(fdOut);
}


bool Connection::sendData(uint32_t size, void* buffer) {
    int bytesSent = ::send(fdOut, buffer, size, 0);
    if (bytesSent == -1) {
        std::cerr << "Error sending data" << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return false;
    }
    return true;
}

bool Connection::sendString(const std::string& str) {
    int bytesSent = ::send(fdOut, str.c_str(), str.length(), 0);
    if (bytesSent == -1) {
        std::cerr << "Error sending string" << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return false;
    }
    return true;
}


bool Connection::sendResponceStruct(const responce res) {
    int bytesSent = ::send(fdOut, &res, sizeof(res), 0);
    if (bytesSent == -1) {
        std::cerr << "Error sending responce" << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return false;
    }
    return true;
}

bool Connection::sendResponce(uint32_t status, const std::string& msg) {
    uint32_t dataLen = msg.length();

    if(!sendResponceStruct((responce){.dataLen = dataLen, .status = status})) {
        return false;
    }
    if (!sendString(msg)) {
        return false;
    }
    return true;
}


bool Connection::recvCommand(command& cmd)
{
    if (recv(fdIn, &cmd, sizeof(cmd), 0) == -1) {
        std::cerr << "Receive command failed" << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return false;
    }

    return true;
}



bool Connection::recvData(uint32_t size, char* buffer)
{
    if (recv(fdIn, buffer, size, 0) == -1) {
        std::cerr << "Receive data failed" << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return false;
    }
    return true;
}

bool Connection::sendFile(std::string filePath)
{
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return false;
    }
    const std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    uint32_t fileSize = buffer.size();
    if ((::send(fdOut, &fileSize, sizeof(fileSize), MSG_MORE)) == -1) {
        std::cerr << "Error msg length" << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl;
    }
    file.close();

    if ((::send(fdOut, buffer.data(), fileSize, 0)) == -1) {
        std::cerr << "Error msg length" << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl;
    }

    return true;
}

