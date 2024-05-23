#include "HiddenFileHandler.h"

HiddenFileHandler::HiddenFileHandler(std::string dirName) : name(dirName) {}

// Method to list the files in the directory
void HiddenFileHandler::listFiles() {
    for (const auto& entry : fs::directory_iterator(name))
        std::cout << entry.path() << std::endl;
}

int HiddenFileHandler::runFile(std::string filename) {
    std::string filePath = name + "/" + filename;
    int exit_code = std::system(filePath.c_str());
    return 0;
}

// Method to remove a file from the directory
int HiddenFileHandler::removeFile(std::string filename) {
    std::string filePath = name + "/" + filename;
    if (std::remove(filePath.c_str()) != 0) {
        std::cerr << "Error deleting file: " << filename << std::endl;
        return 1;
    }
    else {
        std::cout << "File deleted successfully" << filename << std::endl;
        return 0;
    }
}

int HiddenFileHandler::putBytesInFile(std::string filename, const std::string& content) {
    std::string filePath = name + "/" + filename;
    std::ofstream outFile;
    if (std::filesystem::exists(filePath)) {
        outFile.open(filePath, std::ios_base::app);
    }
    else {
        outFile.open(filePath); // open and overwrite file
    }

    if (!outFile) {
        //return FILE_NOT_OPEN_ERROR;
        return 1;
    }

    outFile << content;

    if (!outFile) {
       //return FILE_WRITE_ERROR;
        return 1;
    }
    outFile.close();

    //return SUCCSESS;
    return 0;
}
