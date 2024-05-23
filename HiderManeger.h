#include <fstream>

#include "Errors.h"

class HiderManeger
{
private:
    std::string hiderPath;

public:
    HiderManeger(std::string hiderPath);
    ~HiderManeger();


    Error activateHider();

    Error deactivateHider();

    Error hiddenUpload();

    Error hiddenDelete();

    Error hiddenRun();

    Error hiddenList();

};

