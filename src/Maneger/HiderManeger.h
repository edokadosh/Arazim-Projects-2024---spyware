/*

HIDER USE INSTRUCTIONS:

HiderManeger hiderManager = HiderManeger();
WRITE_FILE
HIDER_SETUP
HIDER_UPLOAD
...
...

*/

#ifndef HIDER_MANEGER_H
#define HIDER_MANEGER_H

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <memory>
#include <vector>
#include <string>
#include <sstream>

#include "FunCodes.h"
#include "Connection.h"
#include "../IncludeCPP/Status.h"
#include "../Hider/HiderCodes.h"
#include "../IncludeCPP/encoding.h"
#include "../IncludeCPP/globalDefines.h"
#include "PipeConnection.h"
#include "BufferConnection.h"

#define DEFAULT_HIDER_PATH ("./matbuja")



class HiderManeger
{
private:
    static HiderManeger* instancePtr;

    std::string hiderPath;
    std::string imagePath;
    std::string mountPath;

    int mthpipe[2]; // pipe from manager -> hider
    bool MtHredirect;
    int htmpipe[2]; // pipe from hider -> manager
    bool HtMredirect;

    Status activateHider(const command& cmd);

    void activateHiderChild(const command& cmd);
    
    Status openPipes(int p[]);

    Status hiddenUpload(const command& cmd, std::shared_ptr<Connection> conn);

    Status hiddenRetrieve(std::shared_ptr<Connection> conn);

    Status hiddenList(std::shared_ptr<Connection> conn);

    std::vector<std::string> split(const std::string &str, char delimiter);

    HiderManeger();

public:
    ~HiderManeger();
    HiderManeger(HiderManeger const&) = delete;
    void operator=(HiderManeger const&)  = delete;
    
    static HiderManeger& getInstance()
    {
        static HiderManeger    instance; // Guaranteed to be destroyed.
                                // Instantiated on first use.
        return instance;
    }
    
    Status setUpHider(std::string hiderPath);

    Status hiddenAction(const command& cmd, std::shared_ptr<Connection> conn);

    Status hideFile(const std::string filename, std::string identifier);

    Status writeFile(const std::string& fileName, char buffer[], uint32_t len, uint32_t writeMode);
};

#endif
