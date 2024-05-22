#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

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
        void recvData(char buffer[]);
        void sendData(const char* msg);
        void closeSock();
        void recvFile();
        void sendFile();
};
