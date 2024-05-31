#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <sys/stat.h>

// #define TESTING_MODE

#include "Status.h"
#include "SoftwareManeger.h"
#include "testing.h"
#include "client.h"

#define SOFTWARE_DIR_PATH ("./")

SoftwareManeger::SoftwareManeger(void) {};

SoftwareManeger::~SoftwareManeger(void) {};



Status SoftwareManeger::fileWrite(Client& client, std::string fileName)
{
    char fileContent[BUFFER_SIZE] = "debug debug";
    Status res = SUCCSESS;
    
    std::cout << fileName << std::endl;

    std::string filePath = SOFTWARE_DIR_PATH  + fileName;
    std::ofstream outFile;
    outFile.open(filePath, std::ios::binary | std::ios::out); // open and overwrite file
    

    if (!outFile) {
        return FILE_NOT_OPEN_ERROR;
    }
    std::cout << "tring to recv data\n";
    while (client.recvData(fileContent) > 0 && res == SUCCSESS)
    {
        std::cout << "recived data\n";
        std::cout << fileContent;
        outFile << fileContent;
        if (!outFile) {
            std::cerr << "FILE_WRITE_ERROR\n"; 
            return FILE_WRITE_ERROR;
        }
    }
    std::cout << "finnish loop\n";
    if (res != SUCCSESS) {
        std::cerr << "res: " << res << std::endl;
        return res;
    }

    outFile.close();
    std::cout << "file closed seccsefuly\n";

    // TODO delete this, debug only
    std::ifstream inputFile("1234.cpp");
    
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return FILE_NOT_OPEN_ERROR;
    }
    
    // Read from the file line by line
    std::string line;
    while (std::getline(inputFile, line)) {
        // Process each line (in this example, just print it)
        std::cout << "printing line\n";
        std::cout << line << std::endl;
    }
    // end TODO delete this, debug only


    if (chmod(filePath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO) != 0)
    {
        std::cerr << "Failed to set executable permission." << std::endl;
        return CHMOD_TO_EXE_ERROR;
    }
    return SUCCSESS;
}


Status SoftwareManeger::deleteFile(const std::string& fileName)
{
    std::string filePath = SOFTWARE_DIR_PATH  + fileName;
    if (std::remove(filePath.c_str()) != 0) {
        return FILE_DELETION_ERROR;
    }
    return SUCCSESS;
}

#ifdef TESTING_MODE

void test_chunkWrite(SoftwareManeger swm)
{
    std::string fileName = "tempChunkWriteTest.txt";
    std::string filePath = SOFTWARE_DIR_PATH + fileName;
    IS_TRUE(swm.chunkWrite(fileName, false, std::string(CHUNK_SIZE, 'A').c_str()) == SUCCSESS);
    IS_TRUE(swm.chunkWrite(fileName, true, std::string(CHUNK_SIZE, 'B').c_str()) == SUCCSESS);
    
    
    std::ifstream file(filePath, std::ios::binary);
    std::string fileContent;
    getline(file, fileContent);
    IS_TRUE(fileContent == std::string(CHUNK_SIZE, 'A') + std::string(CHUNK_SIZE, 'B'));
}

int main()
{
    SoftwareManeger swm = SoftwareManeger();
    test_chunkWrite(swm);
    return 0;
}
#endif

