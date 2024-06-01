/*

USE INSRUCTIONS:

Client client;
client.createSocket();
client.connectServer();
...
...
client.closeSock();

(main example in cpp file)

*/

#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#include "../../Message.pb.h"

#ifndef MANEGER_CLIENT_H
#define MANEGER_CLIENT_H

#define BUFFER_SIZE 1024
#define PORT 23456
#define HOST "localhost"
// #define SOCK_TIMEOUT (5)

class Client
{
    private:
        int clientSocket;
        struct sockaddr_in clientAddr;

        void setTimeout(int t);

    public:
        void createSocket();
        void bindSocket();
        int  acceptConnection();
        void connectServer();
        int  recvData(char buffer[]);
        void sendData(const char* msg);
        void closeSock();
        void recvFile();
        void sendFile(const std::string& fileName);
        void recvCommand(Message& msg);
        void sendCommand(Message& msg);
};
#endif