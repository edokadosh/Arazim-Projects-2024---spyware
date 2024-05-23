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
	const std::string name="./some_thing";
public:

	void listFiles();

	int removeFile(std::string filename);

	int runFile(std::string filenam);

	int putBytesInFile(std::string filename, const std::string& content);
};

#endif