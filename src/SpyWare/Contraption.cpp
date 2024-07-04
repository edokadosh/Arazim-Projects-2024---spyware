#include "Contraption.h"

int Contraption::writeFile(const std::string& fileName, char buffer[], uint32_t len, uint32_t writeMod) 
{
    HiderManeger& hiderManeger = HiderManeger::getInstance();
    return hiderManeger.writeFile(fileName, buffer, len, writeMod);
}
