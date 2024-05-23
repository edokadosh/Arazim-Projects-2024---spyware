#include "HiddenFileHandler.h"
#include "Errors.h"

// Method to list the files in the directory
void HiddenFileHandler::listFiles() {
    for (const auto& entry : fs::directory_iterator(folderName))
        std::cout << entry.path() << std::endl;
}

Status HiddenFileHandler::runFile(const std::string& filename) {
    std::string filePath = folderName + "/" + filename;
    int exit_code = std::system(filePath.c_str());
    return SUCCSESS;
}

// Method to remove a file from the directory
Status HiddenFileHandler::removeFile(const std::string& filename) {
    std::string filePath = folderName + "/" + filename;
    if (std::remove(filePath.c_str()) != 0) {
        return FILE_DELETION_ERROR;
    }
    return SUCCSESS;
}

Status HiddenFileHandler::putBytesInFile(const std::string& filename, const std::string& content) {
    std::string filePath = folderName + "/" + filename;
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
