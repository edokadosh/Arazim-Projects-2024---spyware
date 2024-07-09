#include "PipeConnection.h"

PipeConnection::PipeConnection(int pIn, int pOut, bool closeIn, bool closeOut)
    : Connection(pIn, pOut, closeIn, closeOut) {}

Status PipeConnection::createPipeConnection(std::unique_ptr<PipeConnection>& pipeConn, int fdIn, int fdOut) {
    pipeConn = std::make_unique<PipeConnection>(fdIn, fdOut, true, true);
    return SUCCSESS;
}