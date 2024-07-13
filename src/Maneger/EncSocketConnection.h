#pragma once
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <vector>
#include <memory>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <unistd.h>
#include <iostream>

#include "Connection.h"
#include "../IncludeCPP/Status.h"
#include "command.h"
#include "responce.h"
#include "../IncludeCPP/globalDefines.h"
#include "SocketConnection.h"


using namespace std;

struct SSLCtxDeleter {
    void operator()(SSL_CTX* ctx) const {
        SSL_CTX_free(ctx);
    }
};

struct SSLDeleter {
    void operator()(SSL* ssl) const {
        SSL_shutdown(ssl);
        SSL_free(ssl);
    }
};

struct BIODeleter {
    void operator()(BIO* bio) const {
        BIO_free_all(bio);
    }
};

struct X509Deleter {
    void operator()(X509* x509) const {
        X509_free(x509);
    }
};

class EncSocketConnection : public SocketConnection {

protected:
    unique_ptr<BIO, BIODeleter> bio;
    unique_ptr<X509, X509Deleter> cert;
    unique_ptr<SSL_CTX, SSLCtxDeleter> ctx;
    unique_ptr<SSL, SSLDeleter> ssl;



    int doSend(const void* buf, size_t size, int flags) override;
    int doRecv(void* buf, size_t size, int flags) override;
    static bool isOpensslLibaryInitiated;

public:

    EncSocketConnection(int fdInput, int fdOutput, bool needCloseInput, bool needCloseOutput, struct addrinfo addr,
                        unique_ptr<BIO, BIODeleter> bio_, unique_ptr<X509, X509Deleter> cert_,
                        unique_ptr<SSL_CTX, SSLCtxDeleter> ctx_, unique_ptr<SSL, SSLDeleter> ssl_);

    static int connectTCP(std::string host, int port, std::shared_ptr<EncSocketConnection>& conn);



};

