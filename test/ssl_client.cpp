#include <openssl/ssl.h>
#include <openssl/err.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <iostream>


int main() {
    // Initialize OpenSSL
    SSL_load_error_strings();
    SSL_library_init();
    OpenSSL_add_all_algorithms();

    // Create SSL context
    SSL_CTX* ctx = SSL_CTX_new(SSLv23_client_method());

    // Load server certificate (replace 'server.crt' with your server's certificate)
    if (!SSL_CTX_load_verify_locations(ctx, "server.crt", NULL)) {
        std::cerr << "Error loading server certificate." << std::endl;
        return 1;
    }

    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        return 1;
    }

    // Connect to server
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8888);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("Connection failed");
        return 1;
    }

    // Create SSL connection
    SSL* ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sockfd);

    if (SSL_connect(ssl) != 1) {
        std::cerr << "SSL handshake failed." << std::endl;
        SSL_free(ssl);
        close(sockfd);
        return 1;
    }

    // Send data
    const char* msg = "Hello from C++ client";
    SSL_write(ssl, msg, strlen(msg));

    // Receive data
    char buffer[1024];
    int bytes = SSL_read(ssl, buffer, sizeof(buffer));
    if (bytes > 0) {
        buffer[bytes] = '\0';
        std::cout << "Received: " << buffer << std::endl;
    }

    // Clean up
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(sockfd);
    SSL_CTX_free(ctx);
    ERR_free_strings();
    EVP_cleanup();

    return 0;
}
