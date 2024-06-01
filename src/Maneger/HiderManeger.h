/*

HIDER USE INSTRUCTIONS:

HiderManeger hiderManager = HiderManeger();
WRITE_FILE
HIDER_SETUP
HIDER_UPLOAD
...
...

*/


#include <iostream>
#include <fstream>
#include <unistd.h>
#include <unistd.h>
#include <sys/wait.h>

#include "FunCodes.h"
#include "Status.h"
#include "Connection.h"
#include "HiderCodes.h"

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

    void activateHiderChild(uint fncode, std::string param);
    
    Status openPipes(int p[]);

    Status hiddenUpload(std::string param, Connection& conn);

    Status hiddenList(Connection& conn);

public:
    HiderManeger();
    ~HiderManeger();

    Status setUpHider(std::string hiderPath);

    Status hiddenAction(uint action, std::string& param, Connection& conn);



};

#endif