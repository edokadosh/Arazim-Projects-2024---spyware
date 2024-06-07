#include "HiddenFileHandler.h"
#include "../ManegerAndHider/Status.h"
#include "HiderCodes.h"

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