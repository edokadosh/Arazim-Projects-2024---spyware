#include "HiddenFileHandler.h"

#ifndef HIDER_H
#define HIDER_H

class Hider
{
private:
	HiddenFileHandler handel;
public:
	Hider();
	uint manage_files(int argc, char* argv[]);
};
#endif