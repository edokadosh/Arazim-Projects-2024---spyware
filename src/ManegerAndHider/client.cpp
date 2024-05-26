#include "client.h"

// int main() {
//     Client client;
//     client.createSocket();

//     client.connectServer();

//     char buffer[BUFFER_SIZE] = {0};
//     //client.recvData(buffer);
    
//     Message msg;
//     client.recvCommand(msg);

//     std::cout << "Server response: " << msg.fnccode() 
//         << " " << msg.param1() << " " << msg.param2() << " " << msg.param3() << std::endl;
    
//     client.sendCommand(msg);

//     std::cout << "Trying to send file\n";
//     client.sendFile();
//     std::cout << "Try me\n";

//     client.closeSock();

//     return 0;
// }

void Client::createSocket()
{
    // Create a socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error: Could not create socket" << std::endl; // TODO remove this
        exit(1);
    }
    setTimeout(SOCK_TIMEOUT);
}

void Client::connectServer()
{
    // Server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT); // Port number
    serverAddr.sin_addr.s_addr = inet_addr(HOST); // IP address of the server

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error: Connection failed" << std::endl; // TODO remove this
        close(clientSocket);
        exit(1);
    }
}

int Client::recvData(char buffer[]) const
{
    // Receive data from the server
    int bytes_received = 0;
    if ((bytes_received = recv(clientSocket, buffer, BUFFER_SIZE, 0)) == -1) {
        std::cerr << "Error: Receive failed" << std::endl; // TODO remove this
        close(clientSocket);
        exit(1);
    }
    return bytes_received;
}

void Client::sendData(const char* msg)
{
    // Send data to the server
    if (send(clientSocket, msg, strlen(msg), 0) == -1) {
        std::cerr << "Error: Send failed" << std::endl; // TODO remove this
        close(clientSocket);
        return exit(1);
    }
}

void Client::closeSock()
{
    close(clientSocket);
}


void Client::recvFile()
{
     // Open the file for writing
    std::ofstream file("Matbuja.jpg", std::ios::binary);
    if (!file) {
        std::cerr << "Error: Unable to create file" << std::endl; // TODO remove this
        close(clientSocket);
        exit(1);
    }

    // Receive the file
    char buffer[BUFFER_SIZE];
    int bytesReceived;
    while ((bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        std::cout << bytesReceived << "\n";
        file.write(buffer, bytesReceived);
        if (bytesReceived != BUFFER_SIZE)
            break;
    }

    file.close();
}

void Client::sendFile()
{
    // Open the file for reading
    std::ifstream file("Matbuja.jpg", std::ios::binary);
    if (!file) {
        std::cerr << "Error: Unable to open file" << std::endl;
        close(clientSocket);
        exit(1);
    }

    // Send the file
    char buffer[BUFFER_SIZE];
    while (!file.eof()) {
        file.read(buffer, sizeof(buffer));
        send(clientSocket, buffer, file.gcount(), 0);
    }

    file.close();
}

void Client::recvCommand(Message& msg)
{
    char buffer[BUFFER_SIZE];
    int bytes_recv = recvData(buffer);
    
    if (!msg.ParseFromArray(buffer, bytes_recv)) {
        std::cerr << "Failed to parse received data" << std::endl;
        closeSock();
        exit(1);
    }
}

void Client::sendCommand(Message& msg)
{
    // Serialize the message
    std::string serialized_message;
    if (!msg.SerializeToString(&serialized_message)) {
        std::cerr << "Failed to serialize message" << std::endl;
        closeSock();
        exit(1);
    }
    sendData(serialized_message.c_str());
}

void Client::setTimeout(int t)
{
    struct timeval timeout;
    timeout.tv_sec = t;  
    timeout.tv_usec = 0;
    if (setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
        std::cerr << "Error setting socket options\n";
        closeSock();
        exit(1);
    }
}
