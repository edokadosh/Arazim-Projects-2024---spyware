#include "client.h"

int main() {
    Client client;
    client.createSocket();

    client.connectServer();

    char buffer[BUFFER_SIZE] = {0};
    client.recvData(buffer);
    std::cout << "Server response: " << buffer << std::endl;
    
    client.sendData("CYBER");

    std::cout << "Trying to send file\n";
    client.sendFile();
    std::cout << "Try me\n";

    client.closeSock();

    return 0;
}

void Client::createSocket()
{
    // Create a socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error: Could not create socket" << std::endl;
        exit(1);
    }
}

void Client::connectServer()
{
    // Server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT); // Port number
    serverAddr.sin_addr.s_addr = inet_addr(HOST); // IP address of the server

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error: Connection failed" << std::endl;
        close(clientSocket);
        exit(1);
    }
}

void Client::recvData(char buffer[])
{
    // Receive data from the server
    if (recv(clientSocket, buffer, BUFFER_SIZE, 0) == -1) {
        std::cerr << "Error: Receive failed" << std::endl;
        close(clientSocket);
        exit(1);
    }
}

void Client::sendData(const char* msg)
{
    // Send data to the server
    if (send(clientSocket, msg, strlen(msg), 0) == -1) {
        std::cerr << "Error: Send failed" << std::endl;
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
        std::cerr << "Error: Unable to create file" << std::endl;
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
