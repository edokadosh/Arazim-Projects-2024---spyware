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
#include <contrapMeta.h>

#ifndef HIDDEN_FILE_HANDLER_H
#define HIDDEN_FILE_HANDLER_H



namespace fs = std::filesystem;

class HiddenFileHandler
{
private:
	std::string folderName;
	std::string imagePath;
	std::string getPath(const std::string fileName);

public:

	HiddenFileHandler() {
		//fs::path folder = fs::current_path();
		//folderName = folder.string();
		folderName = "NDA";
		imagePath = "NDA";
	}
	
	void listFiles();

	void setFolderName(const std::string& name);

	Status runFile(const std::string& filename);

	Status deleteFile(const std::string& filename);

	Status uploadFile(const std::string& filename, uint32_t fileSize);

	Status retreiveFile(const std::string& filename);

	Status writeFile(const std::string& fileName, char buffer[], uint32_t len, WriteMod writeMod);
};

#endif