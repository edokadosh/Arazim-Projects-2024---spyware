#include "Connection.h"


Connection::Connection(int socket, struct sockaddr_in address)
{
    socket_ = socket;
    address_ = address;
}

Connection::~Connection() {
    close(socket_);
}

bool Connection::sendResponce() {

}

bool Connection::sendData(const std::string& msg)
{
    int bytesSent;
    int32_t length = htonl(msg.length());
    
    // TODO pre-send full responce struct

    bytesSent = ::send(socket_, &length, sizeof(length), 0);
    
    if (bytesSent == -1) {
        std::cerr << "Error sending data length" << std::endl;
        return false;
    }
    

    bytesSent = ::send(socket_, msg.c_str(), msg.length(), 0);
    if (bytesSent == -1) {
        std::cerr << "Error sending data" << std::endl;
        return false;
    }
    return true;
}



bool Connection::recvCommand(command& cmd)
{
    if (recv(socket_, &cmd, sizeof(cmd), 0) == -1) {
        std::cerr << "Receive command failed" << std::endl;
        return false;
    }
    return true;
}

bool Connection::recvData(char data[CHUNK_SIZE])
{
    if (recv(socket_, data, sizeof(data), 0) == -1) {
        std::cerr << "Receive data failed" << std::endl;
        return false;
    }
    return true;
}



