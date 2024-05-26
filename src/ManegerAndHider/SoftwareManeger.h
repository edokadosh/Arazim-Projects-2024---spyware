#include <string>

#include "Status.h"
#include "client.h"


#ifndef SOFTWARE_MANEGER_H
#define SOFTWARE_MANEGER_H

#define CHUNK_SIZE (1024)



class SoftwareManeger {


public:

    SoftwareManeger();

    ~SoftwareManeger();    

    Status fileWrite(Client& client, std::string param);

    Status deleteFile(const std::string& fileName);
};

#endif