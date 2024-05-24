#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

#include "HiderManeger.h"
#include "HiderCodes.h"

HiderManeger::HiderManeger() :  hiderPath(DEFAULT_HIDER_PATH), \
                                htmpipe({-1, -1}), \
                                mthpipe({-1, -1}), \
                                HtMredirect(false), \
                                MtHredirect(false) {}

Status HiderManeger::setUpHider(std::string path)
{
    hiderPath = path;
}

void HiderManeger::activateHiderChild(uint fncode, std::string param) {
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

Status HiderManeger::activateHider(uint fncode, std::string param)
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

Status HiderManeger::openPipes(int p[])
{
    if (pipe(p) == -1) {
        return HIDER_PIPE_ERROR;
    }
    return SUCCSESS;
}

// add Status handling
Status HiderManeger::hiddenAction(uint action, std::string& param, Client& client)
{
    if (action & HIDDEN_UPLOAD) {
        openPipes(mthpipe); // add Status handling
        MtHredirect = true;
    }
    if (action & HIDDEN_LIST) {
        openPipes(htmpipe); // add Status handling
        HtMredirect = true;
    }

    activateHider(action, param);
    
    if (action & HIDDEN_UPLOAD) {
        close(mthpipe[0]);
        MtHredirect = false;
    }
    if (action & HIDDEN_LIST) {
        close(htmpipe[1]);
        HtMredirect = false;
    }

    if (action & HIDDEN_UPLOAD) {
        hiddenUpload(param, client);
    }

    if (action & HIDDEN_LIST) {
        hiddenList(client);
    }
}

Status HiderManeger::hiddenUpload(std::string param, Client& client)
{

    // send file server -> pipe
    char buffer[BUFFER_SIZE] = { 0 };
    while (true)
    {
        int bytes_received = client.recvData(buffer);
        if (bytes_received <= 0)
            break;
        write(mthpipe[1], buffer, bytes_received); // add Status handling
    }

    close(mthpipe[1]);
    
    return SUCCSESS;
}


Status HiderManeger::hiddenList(Client& client)
{
    // loop for receiving file pipe -> server
    bool cont = true;
    char buffer[BUFFER_SIZE] = {0};
    while (cont)
    {
        if (read(htmpipe[0], buffer, BUFFER_SIZE) != BUFFER_SIZE) // add Status handling
            cont = false;
        client.sendData(buffer);
    }
    
    close(htmpipe[0]);

    return SUCCSESS;
}