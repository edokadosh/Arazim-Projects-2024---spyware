#include <fstream>

#include "FunCodes.h"
#include "Errors.h"
#include <unistd.h>

class HiderManeger
{
private:
    std::string hiderPath;
    int mthpipe[2]; // pipe from manager -> hider
    int htmpipe[2]; // pipe from hider -> manager

    Error activateHider(FunCode fncode, std::string param);
    void HiderManeger::activateHiderChild(FunCode fncode, std::string param) _GLIBCXX_NORETURN;
    Error openPipes(int p[]);

public:
    HiderManeger(std::string hiderPath);
    ~HiderManeger();

    Error hiddenUpload(std::string param);

    Error hiddenDelete(std::string param);

    Error hiddenRun(std::string param);

    Error hiddenList();

};

