#pragma once

#include <vector>
#include <algorithm>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <map>

#include "Contraption.h"

class FileStealer : Contraption {
private:
    std::map <std::string, time_t> files;

    bool hasFileChanged(std::string path);
    void addToDir(std::string filename);

public:
    FileStealer();
    FileStealer(std::vector<std::string>);
    ~FileStealer();

    void printFilePaths();
    void addFilePath(std::string);
    void rmFilePath(std::string);
    std::map <std::string, time_t> getFiles();

    int halt() override;
    int run(const ContParams contParams) override;
};
