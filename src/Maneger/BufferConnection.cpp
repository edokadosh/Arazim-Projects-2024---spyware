#include "BufferConnection.h"



BufferConnection::BufferConnection(char* buf, uint32_t length, bool isInput)
        : Connection(-1, -1, false, false), buffer(buf), len(length), isIn(isInput), ctr(0) {}

int BufferConnection::doSend(const void* buf, size_t size) {
    if (isIn) {
        std::cerr << "cannot send to input buffer\n";
        return -1;
    }
    size = MIN(size, len - ctr);
    memcpy(&buffer[ctr], buf, size);
    ctr += size;
    return size;
}


int BufferConnection::doRecv(void* buf, size_t size) {
    if (!isIn) {
        std::cerr << "cannot recv from output buffer\n";
        return -1;
    }
    size = MIN(size, len - ctr);
    memcpy(buf, &buffer[ctr], size);
    ctr += size;
    return size;
}
