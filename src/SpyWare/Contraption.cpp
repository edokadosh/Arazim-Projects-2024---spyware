#include "Contraption.h"

int Contraption::writeFile(const std::string& fileName, char buffer[], uint32_t len) 
{
    HiderManeger& hiderManeger = HiderManeger::getInstance();
    return hiderManeger.writeFile(fileName, buffer, len);
}
