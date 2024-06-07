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

// void HiderManeger::activateHiderChild(uint fncode, std::string param) {
//     char numericalArg[3];
//     sprintf(numericalArg, "%d", fncode);

//     if (MtHredirect) {
//         close(mthpipe[1]);
//         dup2(mthpipe[0], STDIN_FILENO);
//         close(mthpipe[0]);
//     }
//     if (HtMredirect) {
//         close(htmpipe[0]);
//         dup2(htmpipe[1], STDOUT_FILENO);
//         close(htmpipe[1]);
//     }
    
//     execl(hiderPath.c_str(), numericalArg, param, NULL);
// }

void HiderManeger::activateHiderChild(const command& cmd, int fdIn, int fdOut) {
    std::string encFunCode = encodeInt(cmd.fncode);
    std::string encDataLen = encodeInt(cmd.dataLen);
    std::string encStrParam = encodeStr(cmd.strParam);


    if (fdIn >= 0) {
        dup2(fdIn, STDIN_FILENO);
    }
    if (fdOut >= 0) {
        dup2(fdOut, STDOUT_FILENO);
    }
    
    execl(hiderPath.c_str(), encFunCode.c_str(), encDataLen.c_str(), encStrParam.c_str(), NULL);
}


Status HiderManeger::activateHider(const command& cmd, int fdIn, int fdOut)
{
    pid_t pid = fork();
    if (pid == -1) {
        return HIDER_FORK_ERROR;
    }
    if (pid == 0) { // child work
        activateHiderChild(cmd, fdIn, fdOut);
    }
    if (waitChild(pid) != 0) {
        return WAIT_ERROR;
    }
    return SUCCSESS;
}

Status HiderManeger::openPipes(int p[])
{
    if (pipe(p) == -1) {
        return HIDER_PIPE_ERROR;
    }
    return SUCCSESS;
}

// add Status handling
Status HiderManeger::hiddenAction(const command& cmd, Connection& conn)
{
    return activateHider(cmd, conn.socket_, conn.socket_);   
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
    bool cont = true;
    char buffer[CHUNK_SIZE] = {0};
    while (cont)
    {
        if (read(htmpipe[0], buffer, sizeof(buffer)) != sizeof(buffer)) // add Status handling
            cont = false;
        conn.sendString(buffer);
    }
    
    close(htmpipe[0]);

    return SUCCSESS;
}