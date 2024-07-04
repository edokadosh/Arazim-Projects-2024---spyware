#include "PipeConnection.h"

PipeConnection::PipeConnection(int pIn, int pOut, bool closeIn, bool closeOut)
    : Connection(pIn, pOut, closeIn, closeOut) {}

Status PipeConnection::createPipeConnection(std::shared_ptr<PipeConnection>& pipeConn, int p[2]) {
    pipeConn = std::make_shared<PipeConnection>(p[0], p[1], true, true);
    return SUCCSESS;
}