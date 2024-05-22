#include <string>

#include "Errors.h"


#define CHUNK_SIZE (1024)

#define FILES_CAPCITY (50)

struct Metadata {
    std::string files[FILES_CAPCITY];

};

class SoftwareManeger {


public:

    SoftwareManeger(const std::string& folderName);
    
    Error writeFile(const std::string& fileName, bool isAppend, const char fileContent[CHUNK_SIZE]);



    Error deleteFile(const std::string& fileName);
    
    int runFile();

    int getMetadata();

};