#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <cstring>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../IncludeCPP/Status.h"
#include "../IncludeCPP/globalDefines.h"

#ifndef HIDDEN_FILE_HANDLER_H
#define HIDDEN_FILE_HANDLER_H

#define OUTPUT_PIPE_FD (6)

namespace fs = std::filesystem;

class HiddenFileHandler
{
private:
	std::string folderName;
	std::string imagePath;
	std::string getPath(const std::string fileName);

public:

	HiddenFileHandler(); 
	
	Status listFiles();

	Status runFile(const std::string& filename);

	Status deleteFile(const std::string& filename);

	Status uploadFile(const std::string& filename, uint32_t fileSize);

	Status retreiveFile(const std::string& filename);

	void setFolderName(const std::string& name);
};

#endif