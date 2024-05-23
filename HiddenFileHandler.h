#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

class HiddenFileHandler
{
private:
	const std::string name="./some thing";
public:

	void listFiles();

	int removeFile(std::string filename);

	int runFile(std::string filenam);

	int putBytesInFile(std::string filename, const std::string& content);
};