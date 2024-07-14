#include "EncSocketConnection.h"

bool EncSocketConnection::isOpensslLibaryInitiated = false;

const char* cert_data = "-----BEGIN CERTIFICATE-----\n"
"MIIDSzCCAjMCFFJBNqP3Podgjrwj/GSre9SjyVP+MA0GCSqGSIb3DQEBCwUAMGIx\n"
"CzAJBgNVBAYTAklTMQswCQYDVQQIDAJUVjELMAkGA1UEBwwCVFYxCzAJBgNVBAoM\n"
"AlNTMQwwCgYDVQQLDANTU1MxCzAJBgNVBAMMAkFTMREwDwYJKoZIhvcNAQkBFgJB\n"
"UDAeFw0yNDA3MDkxNjAzNTFaFw0yNTA3MDkxNjAzNTFaMGIxCzAJBgNVBAYTAklT\n"
"MQswCQYDVQQIDAJUVjELMAkGA1UEBwwCVFYxCzAJBgNVBAoMAlNTMQwwCgYDVQQL\n"
"DANTU1MxCzAJBgNVBAMMAkFTMREwDwYJKoZIhvcNAQkBFgJBUDCCASIwDQYJKoZI\n"
"hvcNAQEBBQADggEPADCCAQoCggEBAMCuHKV0N3PEOXazTHTYuUx/k0JVLIVPdnu4\n"
"D68uQazyC3wRi+5fHzuWKe59mmmV7jaU+gGcILMg2n0UYLeJTKeHTzYTduNOAIjc\n"
"k7e/PKR7mat2I8z6fYIyy1uCzDzajGCKxB1RLhH5e1I9d8EfroJA1fD+pDnKsh61\n"
"Oh9KgcTWrO2Z70nntbg026ks3cz++VxHugcpZdgrJd8qGD8QOwudOVGaGxztFBTs\n"
"9DqyPxg/KCT9oJmiwkjaTSbx4IvSh+eykXlRQtKazXmqGBIPrEN26NxRMP+EqApv\n"
"/xaFM8BtPBz183Ek+5PWFbos7Kmoq0uLp+N3WZ+Z+SVb73VYP0kCAwEAATANBgkq\n"
"hkiG9w0BAQsFAAOCAQEAtvc2WZevW88T7ZGa/5jO3RUlPKPfDtFgZm5UGRNJ2i+P\n"
"DYYxgLOxMWlQZDQml3D/I6ppShFDCXqc8yFEkaMhL6jEd7LkAUEsIZ18d7CpLC61\n"
"2YzoF+VEfrtDX/K/36F87yTY9+gP4j8xn7NiaA9zzYt5V4MfxoMqf1rESPNf0WrU\n"
"ABwIFA1y/IJCI6VMnCOcOUxQPDTcELa5+arJ6iQYNYbLS+OmyrEbVqfO+APJMC4w\n"
"DmWYKNYtvkaDi/1KG+1TVJzNZYAncB/pGMHfu50sAKKJAC0u8B7nysq0XTTbTU43\n"
"UYhTU8+alq3+QnDSoCP2ceD/7C1zApin8zo4RmwK1w==\n"
"-----END CERTIFICATE-----\n";


int EncSocketConnection::doSend(const void* buf, size_t size, int flags) {
    return SSL_write(ssl.get(), buf, size);
}

int EncSocketConnection::doRecv(void* buf, size_t size, int flags) {
    return SSL_read(ssl.get(), buf, size);
}

using namespace std;

EncSocketConnection::EncSocketConnection(int fdInput, int fdOutput, bool needCloseInput, bool needCloseOutput, addrinfo addr, \
                        unique_ptr<BIO, BIODeleter> bio_, unique_ptr<X509, X509Deleter> cert_, \
                        unique_ptr<SSL_CTX, SSLCtxDeleter> ctx_, unique_ptr<SSL, SSLDeleter> ssl_) 
                        : SocketConnection(fdInput, fdOutput, needCloseInput, needCloseOutput, addr), 
                        bio(move(bio_)), cert(move(cert_)), ctx(move(ctx)), ssl(move(ssl_)) {}


bool EncSocketConnection::checkShutdown() {
    int sslShutdownStatus = SSL_get_shutdown(ssl.get());
    return sslShutdownStatus & SSL_RECEIVED_SHUTDOWN;
}

int EncSocketConnection::connectEncTCP(string host, int port, shared_ptr<EncSocketConnection>& conn) {
    if (!EncSocketConnection::isOpensslLibaryInitiated) {        
        SSL_load_error_strings();
        SSL_library_init();
        OpenSSL_add_all_algorithms();
        EncSocketConnection::isOpensslLibaryInitiated = true;
    }

    unique_ptr<BIO, BIODeleter> bio(BIO_new_mem_buf((void*)cert_data, -1));
    unique_ptr<X509, X509Deleter> cert(PEM_read_bio_X509(bio.get(), nullptr, nullptr, nullptr));
    if (!cert) {
        std::cerr << "Error loading certificate from string." << std::endl;
        return 1;
    }

    unique_ptr<SSL_CTX, SSLCtxDeleter> ctx(SSL_CTX_new(SSLv23_client_method()));
    if (!ctx) {
        std::cerr << "Error creating SSL context." << std::endl;
        return 1;
    }

    if (SSL_CTX_use_certificate(ctx.get(), cert.get()) != 1) {
        std::cerr << "Error loading certificate into SSL context." << std::endl;
        return 1;
    }

    // Create Socket
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

    std::unique_ptr<SSL, SSLDeleter> ssl(SSL_new(ctx.get()));
    SSL_set_fd(ssl.get(), sockfd);

    if (SSL_connect(ssl.get()) != 1) {
        std::cerr << "SSL handshake failed." << std::endl;
        return 1;
    }

    conn = make_shared<EncSocketConnection>(sockfd, sockfd, true, true, *res, move(bio), move(cert), move(ctx), move(ssl));
    return 0;
}

// int EncSocketConnection::flushInput() {
//     std::cerr << "started enc input flush\n";
//     char buffer[CHUNK_SIZE]; // Adjust buffer size as needed
//     int bytesRead = 0;
    
//     int flags = fcntl(fdIn, F_GETFL, 0);
        
//     if (fcntl(fdIn, F_SETFL, flags | O_NONBLOCK) < 0) {
//         std::cerr << "flush input fcntl failed: " << strerror(errno) << std::endl;
//         exit(1);
//     } 

//     // Loop to read and discard incoming data until no more data is available
//     while (mustRead > 0) {
//         // Data read, but we discard it
//         if ((bytesRead = recvData(MIN(sizeof(buffer), mustRead), buffer, 0)) <= 0) {
//             break;
//         }
//     }
//     // std::cerr << "flush input bytesRead: " << bytesRead << std::endl;
//     if (bytesRead == 0) {
//         if (fcntl(fdIn, F_SETFL, flags) < 0) {
//             std::cerr << "flush input fcntl failed: " << strerror(errno) << std::endl;
//             exit(1);
//         } 
//         return 0;
//     }
//     if (bytesRead < 0) {
//         // Handle error; SSL_read returned an error
//         int sslError = SSL_get_error(ssl.get(), bytesRead);
//         if (sslError != SSL_ERROR_WANT_READ && sslError != SSL_ERROR_WANT_WRITE) {
//             // Handle specific errors if necessary
//             char err_str[256] = {0};  // Buffer to hold the error string
//             ERR_error_string(sslError, buffer);
//             std::cerr << "error flushing input: " << std::string(err_str) << std::endl;
//             if (fcntl(fdIn, F_SETFL, flags) < 0) {
//                 std::cerr << "flush input fcntl failed: " << strerror(errno) << std::endl;
//                 exit(1);
//             } 
//             return -1;
//         }
//     }
//     if (fcntl(fdIn, F_SETFL, flags) < 0) {
//         std::cerr << "flush input fcntl failed: " << strerror(errno) << std::endl;
//         exit(1);
//     } 
//     return 0;
// }

