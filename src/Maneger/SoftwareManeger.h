#include <string>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>

#include "../IncludeCPP/Status.h"
#include "Connection.h"
#include "../IncludeCPP/globalDefines.h"


#ifndef SOFTWARE_MANEGER_H
#define SOFTWARE_MANEGER_H




class SoftwareManeger {

private:

    std::string getPath(const std::string fileName);

public:

    SoftwareManeger();

    ~SoftwareManeger();    

    Status fileWrite(Connection& conn, uint32_t fileSize, std::string fileName);

    Status runFile(const std::string fileName, int argc, char* argv[], int fdIn, int fdOut);

    Status deleteFile(const std::string& fileName);
};

#endif