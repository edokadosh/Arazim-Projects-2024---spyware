#include <string>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>

#include "Status.h"
#include "Connection.h"


#ifndef SOFTWARE_MANEGER_H
#define SOFTWARE_MANEGER_H

#define CHUNK_SIZE (1024)



class SoftwareManeger {


public:

    SoftwareManeger();

    ~SoftwareManeger();    

    Status fileWrite(Connection& conn, uint32_t fileSize, std::string param);

    Status deleteFile(const std::string& fileName);
};

#endif