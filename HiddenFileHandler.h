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

	Status removeFile(const std::string& filename);

	Status runFile(const std::string& filename);

	Status putBytesInFile(const std::string& filename, const std::string& content);
};

#endif