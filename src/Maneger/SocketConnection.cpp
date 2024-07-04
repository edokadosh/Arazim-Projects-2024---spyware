#include "SocketConnection.h"


SocketConnection::SocketConnection(int fdInput, int fdOutput, bool needCloseInput, bool needCloseOutput, struct addrinfo addr)
        : Connection(fdInput, fdOutput, needCloseInput, needCloseOutput), peerAddr(addr) {
            isSocket = true;
        }

int SocketConnection::connectTCP(std::string host, int port, std::shared_ptr<SocketConnection>& conn_ptr)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Failed to create socket\n";
        std::cerr << strerror(errno) << std::endl;
        return -1;
    }
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;     // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP


    int status = getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints, &res);
    if (status != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << "\n";
        close(sockfd);
        return -1;
    }

    if ((status = connect(sockfd, res->ai_addr, res->ai_addrlen)) == -1) {
        std::cerr << "Failed to connect to server\n";
        std::cerr << strerror(errno) << std::endl;
        close(sockfd);
        return -1;
    }

    conn_ptr = std::make_shared<SocketConnection>(sockfd, sockfd, true, true, *res);
    return SUCCSESS;
}
int SocketConnection::flushInput() {
    char buffer[CHUNK_SIZE]; // Adjust buffer size as needed

    // Receive and discard data until the socket buffer is empty
    while (true) {
        ssize_t bytesReceived = doRecv(buffer, sizeof(buffer), MSG_DONTWAIT);

        if (bytesReceived == -1) {
            // Handle recv errors
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // No more data available
                return 0;
            } else {
                std::cerr << "error flushing input: " << strerror(errno) << std::endl;
                return -1;
            }
        } else if (bytesReceived == 0) {
            // Connection closed by remote
            break;
        } else {
            // Data received, discard it
            // You can log or process the discarded data if needed
        }
    }
    return 0;
}
