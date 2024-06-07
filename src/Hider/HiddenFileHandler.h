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
#include "Status.h"
#include "../ManegerAndHider/globalDefines.h"

#ifndef HIDDEN_FILE_HANDLER_H
#define HIDDEN_FILE_HANDLER_H



namespace fs = std::filesystem;

class HiddenFileHandler
{
private:
	const std::string folderName = "./";
	std::string getPath(const std::string fileName);

public:

	void listFiles();

	Status runFile(const std::string& filename);

	Status deleteFile(const std::string& filename);

	Status writeFile(const std::string& filename, uint32_t fileSize);
};

#endif