#include <string>

#include "Errors.h"


#ifndef SOFTWARE_MANEGER_H
#define SOFTWARE_MANEGER_H

#define CHUNK_SIZE (1024)

#define FILES_CAPCITY (50)

struct Metadata {
    std::string files[FILES_CAPCITY];

};

class SoftwareManeger {


public:

    SoftwareManeger();

    ~SoftwareManeger();

    
    Status chunkWrite(const std::string& fileName, bool isAppend, const char fileContent[CHUNK_SIZE]);



    Status deleteFile(const std::string& fileName);
};

#endif