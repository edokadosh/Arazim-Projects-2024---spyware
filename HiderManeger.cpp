#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

#include "HiderManeger.h"

HiderManeger::HiderManeger(std::string path)
{
    hiderPath = path;
}

void HiderManeger::activateHiderChild(FunCode fncode, std::string param) {
    char numericalArg[3];
    sprintf(numericalArg, "%d", fncode);
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
    
    // parent work

    // execl hider as child
    // if pipes are open use dup
    // command line argument - funcode
    // return the return value of exec
}

Error HiderManeger::openPipes(int p[])
{
    if (pipe(p) == -1) {
        return Error::HIDER_PIPE_ERROR;
    }
    return Error::SUCCSESS;
}

Error HiderManeger::hiddenUpload(std::string param)
{
    openPipes(mthpipe);
    activateHider(FunCode::HIDDEN_UPLOAD, param);
    // TODO
    // loop for send and -> pipe file
}

Error HiderManeger::hiddenDelete(std::string param)
{
    return activateHider(FUNCODES_ENUM_H::HIDDEN_DELETE, param);
}

Error HiderManeger::hiddenRun(std::string param)
{
    return activateHider(FUNCODES_ENUM_H::HIDDEN_RUN, param);
}

Error HiderManeger::hiddenList()
{
    openPipes(htmpipe);
    Error er = activateHider(FUNCODES_ENUM_H::HIDDEN_RUN, "");
    if (er) {
        return er;
    }
    // TODO
    // loop for receiving file pipe -> server 
}