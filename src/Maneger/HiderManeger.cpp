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
    hiderPath = path;
    return SUCCSESS;
}


void HiderManeger::activateHiderChild(const command& cmd) {
    std::string encFunCode = encodeInt(cmd.fncode);
    std::string encDataLen = encodeInt(cmd.dataLen);
    std::string encStrParam = encodeStr(cmd.strParam);


    if (MtHredirect) {
        close(mthpipe[1]);
        dup2(mthpipe[0], STDIN_FILENO);
        close(mthpipe[0]);
    }
    if (HtMredirect) {
        close(htmpipe[0]);
        dup2(htmpipe[1], STDOUT_FILENO);
        close(htmpipe[1]);
    }
    
    execl(hiderPath.c_str(), encFunCode.c_str(), encDataLen.c_str(), encStrParam.c_str(), NULL);
}


Status HiderManeger::activateHider(const command& cmd)
{
    if (HtMredirect) {

    }
    
    pid_t pid = fork();
    if (pid == -1) {
        return HIDER_FORK_ERROR;
    }
    if (pid == 0) { // child work
        activateHiderChild(cmd);
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

    return activateHider(cmd);

    if (cmd.fncode & HIDDEN_UPLOAD) {
        hiddenUpload(cmd, conn);
    }


    int bytes_read;
    while ((bytes_read = splice(htmpipe[0], nullptr, conn.socket_, nullptr, 4096, SPLICE_F_MOVE)) > 0) {
        if (bytes_read == -1) {
            std::cerr << "Error splicing data: " << std::strerror(errno) << std::endl;
            return SPLICE_ERROR;
        }
    }

    if (cmd.fncode & HIDDEN_LIST) {
        hiddenList(conn);
    }


    
}




Status HiderManeger::hiddenUpload(const command& cmd, Connection& conn)
{
    std::cout << "UPLOADING " << cmd.strParam << std::endl;
    // send file server -> pipe
    char buffer[CHUNK_SIZE] = { 0 };
    while (true)
    {
        int bytes_received = conn.recvData(sizeof(buffer), buffer);
        if (bytes_received <= 0)
            break;
        write(mthpipe[1], buffer, bytes_received); // TODO add Status handling
    }

    close(mthpipe[1]);
    
    return SUCCSESS;
}


Status HiderManeger::hiddenList(Connection& conn)
{
    // loop for receiving file pipe -> server
    int bytes_read;
    while ((bytes_read = splice(htmpipe[0], nullptr, conn.socket_, nullptr, 4096, SPLICE_F_MOVE)) > 0) {
        if (bytes_read == -1) {
            std::cerr << "Error splicing data: " << std::strerror(errno) << std::endl;
            return SPLICE_ERROR;
        }
    }
    
    close(htmpipe[0]);

    return SUCCSESS;
}