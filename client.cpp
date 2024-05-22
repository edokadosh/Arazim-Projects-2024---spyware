#include <iostream>
#include <string>
// #include <WS2tcpip.h> // For Windows
#include <arpa/inet.h> // For Linux

#pragma comment (lib, "ws2_32.lib") // For Windows

int main() {
    // Initialize Winsock for Windows
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0) {
        std::cerr << "Can't initialize Winsock! Quitting" << std::endl;
        return -1;
    }

    // Create a socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Can't create socket! Quitting" << std::endl;
        return -1;
    }

    // Hint structure for the server we're connecting to
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET; // AF_INET = IPv4 addresses
    serverAddress.sin_port = htons(65432); // Little to big endian conversion

    // Convert IP address from text to binary form
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    int connectionResult = connect(clientSocket, (sockaddr*)&serverAddress, sizeof(serverAddress));
    if (connectionResult == SOCKET_ERROR) {
        std::cerr << "Can't connect to server! Quitting" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    // Send and receive data
    char buffer[4096];
    std::string userInput;

    do {
        std::cout << "> ";
        std::getline(std::cin, userInput);

        if (userInput.size() > 0) {
            // Send the text
            int sendResult = send(clientSocket, userInput.c_str(), userInput.size() + 1, 0);
            if (sendResult != SOCKET_ERROR) {
                // Wait for response
                ZeroMemory(buffer, 4096);
                int bytesReceived = recv(clientSocket, buffer, 4096, 0);
                if (bytesReceived > 0) {
                    std::cout << "SERVER> " << std::string(buffer, 0, bytesReceived) << std::endl;
                }
            }
        }

    } while (userInput.size() > 0);

    // Close the socket
    closesocket(clientSocket);

    // Cleanup
    WSACleanup();

    return 0;
}