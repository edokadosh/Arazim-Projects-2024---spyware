#include "HiddenFileHandler.h"
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
	HiddenFileHandler handel;
public:
	Hider();
	int manage_files(int prosses, std::string nameFlie);
};

