#include <stdint.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

#include "contrapMeta.h"

#ifndef CONTRAPTION_H
#define CONTRAPTION_H



class Contraption {

private:
    
public:

    Contraption() {};

    ~Contraption() {};

    int readFile(const std::string& fileName, char buffer[], uint32_t maxLen);

    int writeFile(const std::string& fileName, char buffer[], uint32_t len, WriteMod writeMod);

    virtual int halt() = 0;

    virtual int run(const ContParams contParams) = 0;

};


#endif