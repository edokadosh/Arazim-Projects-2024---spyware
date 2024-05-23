#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

class FileHandler
{
private:
	std::string name;
public:

	FileHandler(std::string dirName);

	void listFiles();

	int removeFile(std::string filename);

	int runFile(std::string filenam);

	int putBytesInFile(std::string filename, const std::string& content);
};