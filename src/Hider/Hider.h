#include "HiddenFileHandler.h"
#include "../IncludeCPP/Status.h"
#include "HiderCodes.h"
#include "../Maneger/responce.h"
#include "../IncludeCPP/encoding.h"

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