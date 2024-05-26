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
#include "Status.h"
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

    Status activateHider(uint fncode, std::string param);

    void HiderManeger::activateHiderChild(uint fncode, std::string param) _GLIBCXX_NORETURN;
    
    Status openPipes(int p[]);

    Status hiddenUpload(std::string param, Client& client);

    Status hiddenList(Client& client);

public:
    HiderManeger();
    ~HiderManeger();

    Status setUpHider(std::string hiderPath);

    Status hiddenAction(uint action, std::string& param, Client& client);



};

#endif