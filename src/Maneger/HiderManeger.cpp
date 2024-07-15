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

Status HiderManeger::setUpHider(std::string strParam)
{
    this->hiderPath = strParam;
    // std::cerr << "setted hider path: " << hiderPath.c_str() << std::endl;

    return SUCCSESS;
}


// Status HiderManeger::setUpHider(std::string strParam)
// {
    // std::vector<std::string> params = split(strParam, ';');
    // if (params.size() != 3)
        // return HIDER_SETUP_ERROR;

    // this->hiderPath = params.at(0);
    // this->imagePath = params.at(1);
    // this->mountPath = params.at(2);
    // std::cerr << "setted hider path: " << hiderPath.c_str() << std::endl;
    // std::cerr << "setted image path: " << imagePath.c_str() << std::endl;
    // std::cerr << "setted mount path: " << mountPath.c_str() << std::endl;
    // return SUCCSESS;
// }

Status HiderManeger::writeFile(const std::string& fileName, char buffer[], uint32_t len, uint32_t writeMod) {
    command cmd = { 0 };
    Status res = SUCCSESS;
    if (fileName.size() >= sizeof(cmd.strParam)) {
        return FILENAME_TO_LONG;
    }
    cmd = (command){.dataLen = len, .fncode = HIDDEN_OPRATION | HIDDEN_UPLOAD};
    strcpy(cmd.strParam, fileName.c_str());
    int p[2];
    if ((res = openPipes(p)) != SUCCSESS) {
        return res;
    }
    std::shared_ptr<BufferConnection> bufConn = std::make_shared<BufferConnection>(buffer, len, true);

    hiddenAction(cmd, bufConn);
    bufConn->closeConn();

    return SUCCSESS;
}

void HiderManeger::activateHiderChild(const command& cmd) {
    std::string encFunCode = encodeInt(cmd.fncode);
    std::string encDataLen = encodeInt(cmd.dataLen);
    std::string encStrParam = encodeStr(cmd.strParam);
    std::string encMountPath = encodeStr(mountPath);
    

    std::cerr << "hider path: " << this->hiderPath.c_str() << std::endl;

    if (MtHredirect) {
        close(mthpipe[1]);
        dup2(mthpipe[0], STDIN_FILENO);
        close(mthpipe[0]);
    }
    if (HtMredirect) {
        close(htmpipe[0]);
        dup2(htmpipe[1], 6);
        close(htmpipe[1]);
    }
    // std::cerr << "exec hider\n";
    // std::cerr << "sdfg fncode: " << cmd.fncode << std::endl;
    // std::cerr << "sdfg datalen: " << cmd.dataLen << std::endl;
    // std::cerr << "sdfg strParam: " << cmd.strParam << std::endl;
    // std::cerr << "MountPath " << mountPath << std::endl;
    // std::cerr << "encMountPath " << encMountPath << std::endl;
    if (execl(this->hiderPath.c_str(), encFunCode.c_str(), encDataLen.c_str(), encStrParam.c_str(), encMountPath.c_str(), NULL) == -1) {
        std::cerr << "error executing hider: " << std::strerror(errno) << std::endl;
    }
}


Status HiderManeger::activateHider(const command& cmd)
{
    
    // std::cerr << "forking\n";
    pid_t pid = fork();
    if (pid == -1) {
        return HIDER_FORK_ERROR;
    }
    if (pid == 0) { // child work
        // std::cerr << "started child" << std::endl;
        activateHiderChild(cmd);
    }

    if (HtMredirect) {
        close(htmpipe[1]);
    }

    if (MtHredirect) {
        close(mthpipe[0]);
    }

    PipeConnection::createPipeConnection(pipeConn, htmpipe[0], mthpipe[1]);


    return SUCCSESS;
}

Status HiderManeger::openPipes(int p[])
{
    p[0] = 0;
    p[1] = 0;
    if (pipe(p) == -1) {
        std::cerr << "error opening pipes" << std::endl;
        return HIDER_PIPE_ERROR;
    }
    return SUCCSESS;
}

// add Status handling
Status HiderManeger::hiddenAction(const command& cmd, std::shared_ptr<Connection> conn)
{

    if (openPipes(mthpipe) != SUCCSESS) {
        return HIDER_PIPE_ERROR;
    }
    MtHredirect = true;

    if (openPipes(htmpipe) != SUCCSESS) {
        close(mthpipe[0]);
        close(mthpipe[1]);
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
            int res = 0;
            if ((res = read(htmpipe[0], &hiderRes, sizeof(hiderRes))) < 0) {
                std::cerr << "error reading from hider pipe" << std::endl;
                return READ_FROM_HIDER_ERROR;
            }
            if (res != sizeof(hiderRes)) {
                std::cerr << "weird\n";
            }
            // std::cerr << "responce status from hider: " << hiderRes.status << std::endl;
            conn->sendResponceStruct(hiderRes);
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


Status HiderManeger::hiddenUpload(const command& cmd, std::shared_ptr<Connection> conn)
{
    // std::cerr << "UPLOADING " << cmd.strParam << std::endl;
    // send file server -> pipe
    char buffer[CHUNK_SIZE] = {0};
    uint32_t ctr = 0;
    uint32_t transmitBytes = 0;
    int bytes_received = 0;
    for (ctr = 0; ctr < cmd.dataLen; ctr += bytes_received)
    {
        transmitBytes = MIN(CHUNK_SIZE, cmd.dataLen - ctr);
        if ((bytes_received = conn->recvData(transmitBytes, buffer)) < 0) {
            std::cerr << "Error recv file: " << strerror(errno) << std::endl;
            return ERROR_RECVIVING_FROM_CONNECTION;
        }
        if (pipeConn->sendData(bytes_received, buffer) != bytes_received) {
            std::cerr << "Error writing to hider: " << strerror(errno) << std::endl;
            return ERROR_WRITING_TO_HIDER;
        }
        // if ((bytes_received = splice(conn->fdIn, nullptr, mthpipe[1], nullptr, transmitBytes, SPLICE_F_MOVE)) == -1) {
        //     std::cerr << "Error splicing data to hider: " << std::strerror(errno) << std::endl;
        //     return SPLICE_ERROR;
        // }
        // std::cerr << "sent one page" << std::endl;
        // std::cerr << "hider maneger: finnish upload iter\n";
    }

    close(mthpipe[1]);
    
    return SUCCSESS;
}

Status HiderManeger::hiddenRetrieve(std::shared_ptr<Connection> conn) {
    uint32_t fileSize;
    char fileContent[CHUNK_SIZE];

    if (read(htmpipe[0], &fileSize, sizeof(fileSize)) < 0)
    {
        std::cerr << "error reading from hider pipe" << std::endl;
        return READ_FROM_HIDER_ERROR;
    }
    conn->sendData(sizeof(fileSize), &fileSize);
    uint32_t ctr;
    uint32_t tranmitBytes = 0;
    int recived = 0;
    for (ctr = 0; ctr < fileSize; ctr += recived)
    {
        tranmitBytes = MIN(sizeof(fileContent), fileSize - ctr);
        if ((recived = read(htmpipe[0], fileContent, tranmitBytes)) < 0)
        {
            std::cerr << "error reading from hider pipe" << std::endl;
            return READ_FROM_HIDER_ERROR;
        }
        conn->sendData(tranmitBytes, fileContent);
    }
    
    close(htmpipe[0]);
    return SUCCSESS;
}

Status HiderManeger::hiddenList(std::shared_ptr<Connection> conn)
{
    // loop for receiving file pipe -> server
    int bytes_read;
    while ((bytes_read = splice(htmpipe[0], nullptr, conn->fdOut, nullptr, 4096, SPLICE_F_MOVE)) > 0) {
        if (bytes_read == -1) {
            std::cerr << "Error splicing data: " << std::strerror(errno) << std::endl;
            return SPLICE_ERROR;
        }
    }
    
    close(htmpipe[0]);

    return SUCCSESS;
}

Status HiderManeger::hideFile(const std::string filename, std::string identifier) {
    std::cerr << "HiderManeger::hideFile: hiding " << 
            filename << identifier << "implementation missing" << std::endl;
    // need to call Hider.Hide()

    return SUCCSESS;
}


std::vector<std::string> HiderManeger::split(const std::string &str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string item;

    while (std::getline(ss, item, delimiter)) {
        tokens.push_back(item);
    }

    return tokens;
}
