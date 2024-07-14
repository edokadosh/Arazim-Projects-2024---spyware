#include <iostream>
#include <memory>
#include "../src/Maneger/Connection.h"
#include "../src/Maneger/SocketConnection.h"
#include "../src/Maneger/EncSocketConnection.h"


const string HOST = "localhost";
const int PORT = 4545;

using namespace std;

int main() {
    shared_ptr<EncSocketConnection> conn;
    EncSocketConnection::connectTCP(HOST, PORT, conn);
    char data[200] = {0};
    conn->recvData(29, data);
    cout << "recved data: " << data << endl;

    const char* responce = "responce from cpp";
    conn->sendData((size_t)strlen(responce), (void*)responce, 0);

    return 0;  
}