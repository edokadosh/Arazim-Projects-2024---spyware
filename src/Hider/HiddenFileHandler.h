#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <filesystem>

#ifndef HIDDEN_FILE_HANDLER_H
#define HIDDEN_FILE_HANDLER_H
#include "Status.h"


namespace fs = std::filesystem;

class HiddenFileHandler
{
private:
	const std::string folderName = "./some_thing";
public:

	void listFiles();

	Status runFile(const std::string& filename);

	Status removeFile(const std::string& filename);

	Status putBytesInFile(const std::string& filename, const std::string& content);
};

#endif