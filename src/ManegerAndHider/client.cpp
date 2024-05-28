#include <cerrno>
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
    serverSocket = -1;
    if (clientSocket == -1) {
        std::cerr << "Error: Could not create socket" << std::endl; // TODO remove this
        std::cerr << std::strerror(errno) << std::endl;
        exit(1);
    }
    // setTimeout(SOCK_TIMEOUT);
}

void Client::bindSocket()
{
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(PORT); // Port number
    clientAddr.sin_addr.s_addr = INADDR_ANY; 

    if (bind(clientSocket, (struct sockaddr *) &clientAddr, sizeof(clientAddr)) == -1) {
        std::cerr << "Error: Bind failed." << std::endl;
        std::cerr << std::strerror(errno) << std::endl;
        close(clientSocket);
        exit(1);
    }
    if (listen(clientSocket, 5) == -1) {
        std::cerr << "Error: Listen failed." << std::endl;
        std::cerr << std::strerror(errno) << std::endl;
        close(clientSocket);
        exit(1);
    }
}

int Client::acceptConnection()
{
    sockaddr_in serverAddress;
    socklen_t serverAddrSize = sizeof(serverAddress);
    
    if ((serverSocket = accept(clientSocket, reinterpret_cast<struct sockaddr*>(&serverAddress), &serverAddrSize)) == -1) {
        std::cerr << "Error: Accept failed." << std::endl;
        std::cerr << std::strerror(errno) << std::endl;
        close(clientSocket);
        return 1;
    }
    char serverAddressStr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(serverAddress.sin_addr), serverAddressStr, INET_ADDRSTRLEN);
    std::cout << "server Socket at address: " << serverAddressStr << ", port: " << ntohs(serverAddress.sin_port) << std::endl;

    return 0;
}

// might need to remove this
void Client::connectServer()
{
    // Server address
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(PORT); // Port number
    clientAddr.sin_addr.s_addr = inet_addr(HOST); // IP address of the server

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&clientAddr, sizeof(clientAddr)) == -1) {
        std::cerr << "Error: Connection failed" << std::endl; // TODO remove this
        std::cerr << std::strerror(errno) << std::endl;
        close(clientSocket);
        exit(1);
    }
}

int Client::recvData(char buffer[]) 
{
    // DEBUG print bound addr
    sockaddr_in boundAddress;
    socklen_t boundAddressSize = sizeof(boundAddress);
    if (getsockname(clientSocket, (struct sockaddr *)&boundAddress, &boundAddressSize) == -1) {
        std::cerr << "Error getting bound address" << std::endl;
        close(clientSocket);
        return 1;
    }

    // Convert the address to a human-readable string
    char boundAddressStr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(boundAddress.sin_addr), boundAddressStr, INET_ADDRSTRLEN);



    std::cout << "Socket bound to address: " << boundAddressStr << ", port: " << ntohs(boundAddress.sin_port) << std::endl;

    // Receive data from the server
    int bytes_received = 0;
    if ((bytes_received = recv(serverSocket, buffer, BUFFER_SIZE, 0)) == -1) {
        std::cerr << "Error: Receive failed" << std::endl; // TODO remove this
        std::cerr << std::strerror(errno) << std::endl;
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
        std::cerr << std::strerror(errno) << std::endl;
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
        std::cerr << std::strerror(errno) << std::endl;
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

void Client::sendFile(const std::string& fileName)
{
    // Open the file for reading
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Unable to open file" << std::endl;
        std::cerr << std::strerror(errno) << std::endl;
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
    
    std::cout << bytes_recv << std::endl;
    if (!msg.ParseFromArray(buffer, bytes_recv)) {
        std::cerr << "Failed to parse received data" << std::endl;
        std::cerr << std::strerror(errno) << std::endl;
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
        std::cerr << std::strerror(errno) << std::endl;
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
        std::cerr << std::strerror(errno) << std::endl;
        closeSock();
        exit(1);
    }
}
