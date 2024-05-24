#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>

// #define TESTING_MODE

#include "Errors.h"
#include "SoftwareManeger.h"
#include "testing.h"

#define SOFTWARE_DIR_PATH ("./")

SoftwareManeger::SoftwareManeger(void) {};

SoftwareManeger::~SoftwareManeger(void) {};


Status SoftwareManeger::chunkWrite(const std::string& fileName, bool isAppend, const char fileContent[CHUNK_SIZE])
{
    std::string filePath = SOFTWARE_DIR_PATH  + fileName;
    std::ofstream outFile;
    if (isAppend) {
        outFile.open(filePath, std::ios_base::app);
    }
    else {
        outFile.open(filePath); // open and overwrite file
    }

    if (!outFile) {
        return FILE_NOT_OPEN_ERROR;
    }

    outFile << fileContent;

    if (!outFile) {
        return FILE_WRITE_ERROR;
    }
    outFile.close();

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

