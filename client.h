#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include "Message.pb.h"

#define BUFFER_SIZE 1024
#define PORT 65432
#define HOST "192.168.154.1"

class Client
{
    int clientSocket;
    struct sockaddr_in serverAddr;

    public:
        void createSocket();
        void connectServer();
        int recvData(char buffer[]);
        void sendData(const char* msg);
        void closeSock();
        void recvFile();
        void sendFile();
        void recvCommand(Message& msg);
        void sendCommand(Message& msg);
};
