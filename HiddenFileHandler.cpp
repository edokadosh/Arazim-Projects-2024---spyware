#include "HiddenFileHandler.h"
#include "Errors.h"

// Method to list the files in the directory
void HiddenFileHandler::listFiles() {
    for (const auto& entry : fs::directory_iterator(name))
        std::cout << entry.path() << std::endl;
}

Status HiddenFileHandler::runFile(std::string filename) {
    std::string filePath = name + "/" + filename;
    int exit_code = std::system(filePath.c_str());
    return SUCCSESS;
}

// Method to remove a file from the directory
Status HiddenFileHandler::removeFile(std::string filename) {
    std::string filePath = name + "/" + filename;
    if (std::remove(filePath.c_str()) != 0) {
        return FILE_DELETION_ERROR;
    }
    return SUCCSESS;
}

Status HiddenFileHandler::putBytesInFile(std::string filename, const std::string& content) {
    std::string filePath = name + "/" + filename;
    std::ofstream outFile;
    if (std::filesystem::exists(filePath)) {
        outFile.open(filePath, std::ios_base::app);
    }
    else {
        outFile.open(filePath); // open and overwrite file
    }

    if (!outFile) {
        return FILE_NOT_OPEN_ERROR;
    }

    outFile << content;

    if (!outFile) {
        return FILE_WRITE_ERROR;
    }
    outFile.close();

    return SUCCSESS;
}
