
// #define TESTING_MODE


#include "SoftwareManeger.h"

#define SOFTWARE_DIR_PATH ("./")
#define BUFFER_SIZE (1024)
#define MIN(x, y) (((x) < (y)) ? (x): (y))

SoftwareManeger::SoftwareManeger(void) {};

SoftwareManeger::~SoftwareManeger(void) {};


Status SoftwareManeger::fileWrite(Connection& conn, uint32_t fileSize, std::string fileName)
{
    char fileContent[BUFFER_SIZE] = { 0 };
    Status res = SUCCSESS;
    
    std::string filePath = SOFTWARE_DIR_PATH  + fileName; // TODO better path find
    std::ofstream outFile;
    outFile.open(filePath, std::ios::binary | std::ios::out); // open and overwrite file
    

    if (!outFile) {
        return FILE_NOT_OPEN_ERROR;
    }
    uint32_t ctr;
    for (ctr = 0; ctr < fileSize && res == SUCCSESS; ctr += sizeof(fileContent))
    {
        int tranferAmount = MIN(sizeof(fileContent), fileSize - ctr);
        // TODO recv exact amount
        if (!conn.recvData(sizeof(fileContent), fileContent)) {
            std::cerr << "Error reciving file contesnt from socket" << std::endl;
            std::cerr << "Error: " << strerror(errno) << std::endl;
            res = RECV_FILE_CONTENT_ERROR;
        }
        outFile.write(fileContent, tranferAmount);
        if (!outFile) {
            std::cerr << "Error writting to file " << std::endl;
            std::cerr << "Error: " << strerror(errno) << std::endl;
            res = FILE_WRITE_ERROR;
        }
    }
    outFile.close();


    if (res != SUCCSESS) {
        return res;
    }

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

