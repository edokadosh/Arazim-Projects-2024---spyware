#include "FileHandler.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <filesystem>
class Hider
{
private:
	FileHandler handel;
public:
	Hider(std::string nameDirectory);
	int manage_files();
};

