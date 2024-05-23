#include <fstream>

#include "FunCodes.h"
#include "Errors.h"
#include <unistd.h>

class HiderManeger
{
private:
    std::string hiderPath;
    int mthpipe[2]; // pipe from manager -> hider
    bool redirectMtH;
    int htmpipe[2]; // pipe from hider -> manager
    bool redirectHtM;

    Error activateHider(FunCode fncode, std::string param);
    Error openPipes(int p[]);

public:
    HiderManeger(std::string hiderPath);
    ~HiderManeger();

    Error hiddenUpload(std::string param);

    Error hiddenDelete(std::string param);

    Error hiddenRun(std::string param);

    Error hiddenList();

};
