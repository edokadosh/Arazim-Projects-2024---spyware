#include "BufferConnection.h"



BufferConnection::BufferConnection(char* buf, bool isInput)
        : Connection(-1, -1, false, false), buffer(buf), isIn(isInput) {}

int BufferConnection::doSend(const void* buf, size_t size) {
    if (isIn) {
        std::cerr << "cannot send to input buffer\n";
        return -1;
    }
    memcpy(&buffer[ctr], buf, size);
    ctr += size;
    return size;
}


int BufferConnection::doRecv(void* buf, size_t size) {
    if (!isIn) {
        std::cerr << "cannot recv from output buffer\n";
        return -1;
    }
    memcpy(buf, &buffer[ctr], size);
    ctr += size;
    return size;
}
