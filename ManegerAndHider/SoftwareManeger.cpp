#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>

#include "Errors.h"

#include "SoftwareManeger.h"

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
