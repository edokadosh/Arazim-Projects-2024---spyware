#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <filesystem>

#ifndef HIDDEN_FILE_HANDLER_H
#define HIDDEN_FILE_HANDLER_H

namespace fs = std::filesystem;

class HiddenFileHandler
{
private:
	const std::string name="./some thing";
public:

	void listFiles();

	Status removeFile(std::string filename);

	Status runFile(std::string filenam);

	Status putBytesInFile(std::string filename, const std::string& content);
};

#endif