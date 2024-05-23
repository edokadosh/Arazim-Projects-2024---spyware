/*

HIDER USE INSTRUCTIONS:

HiderManeger hiderManager = HiderManeger();
WRITE_FILE
HIDER_SETUP
HIDER_UPLOAD
...
...

*/


#include <fstream>
#include <unistd.h>

#include "FunCodes.h"
#include "Errors.h"
#include "client.h"

#ifndef HIDER_MANEGER_H
#define HIDER_MANEGER_H

#define DEFAULT_HIDER_PATH ("./matbuja")


class HiderManeger
{
private:
    std::string hiderPath;
    int mthpipe[2]; // pipe from manager -> hider
    bool MtHredirect;
    int htmpipe[2]; // pipe from hider -> manager
    bool HtMredirect;

    Error activateHider(uint fncode, std::string param);

    void HiderManeger::activateHiderChild(uint fncode, std::string param) _GLIBCXX_NORETURN;
    
    Error openPipes(int p[]);

    Error hiddenUpload(std::string param, Client& client);

    Error hiddenList(Client& client);

public:
    HiderManeger();
    ~HiderManeger();

    Error setUpHider(std::string hiderPath);

    Error hiddenAction(uint action, std::string& param, Client& client);



};

#endif