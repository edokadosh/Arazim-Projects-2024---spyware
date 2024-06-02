#include <cerrno>
#include "Connection.h"
#include "responce.h"

Connection::Connection(int socket, struct sockaddr_in address)
{
    socket_ = socket;
    address_ = address;
    isOpen = true;
}

Connection::Connection()
{
    socket_ = -1;
    isOpen = false;
}

Connection::~Connection() {
    // std::cout << "deastroy" << std::endl;
    // if (socket_ != -1) {
    //     close(socket_);
    // }
    // isOpen = false;
}

void Connection::closeSocket() {
    close(socket_);
    isOpen = false;
}


bool Connection::sendData(uint32_t size, void* buffer) {
    int bytesSent = ::send(socket_, buffer, size, 0);
    if (bytesSent == -1) {
        std::cerr << "Error sending data" << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return false;
    }
    return true;
}

bool Connection::sendString(const std::string& str) {
    int bytesSent = ::send(socket_, str.c_str(), str.length(), 0);
    if (bytesSent == -1) {
        std::cerr << "Error sending string" << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return false;
    }
    return true;
}


bool Connection::sendResponceStruct(const responce res) {
    responce netRes = netEndianResponce(res); 
    int bytesSent = ::send(socket_, &netRes, sizeof(netRes), 0);
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

// bool Connection::sendData(const std::string& msg)
// {
//     int bytesSent;
//     int32_t length = htonl(msg.length());
    
//     // TODO pre-send full responce struct

//     bytesSent = ::send(socket_, &length, sizeof(length), 0);
    
//     if (bytesSent == -1) {
//         std::cerr << "Error sending data length" << std::endl;
//         return false;
//     }
    

//     bytesSent = ::send(socket_, msg.c_str(), msg.length(), 0);
//     if (bytesSent == -1) {
//         std::cerr << "Error sending data" << std::endl;
//         return false;
//     }
//     return true;
// }



bool Connection::recvCommand(command& cmd)
{
    command netCmd;
    if (recv(socket_, &netCmd, sizeof(netCmd), 0) == -1) {
        std::cerr << "Receive command failed" << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return false;
    }
    cmd = hostEndianCommand(netCmd);
    return true;
}



bool Connection::recvData(uint32_t size, char data[])
{
    if (recv(socket_, data, sizeof(data[0]) * size, 0) == -1) {
        std::cerr << "Receive data failed" << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return false;
    }
    return true;
}



