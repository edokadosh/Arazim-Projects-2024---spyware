#include <string>

#include "Status.h"
#include "client.h"


#ifndef SOFTWARE_MANEGER_H
#define SOFTWARE_MANEGER_H

#define CHUNK_SIZE (1024)

#define FILES_CAPCITY (50)


class SoftwareManeger {

private:
    Status chunkWrite(const std::string& fileName, bool isAppend, const char fileContent[CHUNK_SIZE]);

public:

    SoftwareManeger();

    ~SoftwareManeger();    

    Status fileWrite(const Client& client, std::string param);

    Status deleteFile(const std::string& fileName);
};

#endif