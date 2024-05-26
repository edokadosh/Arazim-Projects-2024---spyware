#include "Hider.h"

#include "Status.h"
#include "HiderCodes.h"

Hider::Hider() : handel(HiddenFileHandler()) {}

uint Hider::manage_files(int argc, char* argv[])
{
	std::string line;
	if (argc < 2) {
		return HIDER_NO_ARGUMENTS_ERROR;
	}
	uint fncode = atoi(argv[1]);
	std::string stringParam = nullptr;

	if (argc >= 3) {
		std::string stringParam(argv[2]);
	}
	// TODO better Status handling
	Status res_upload = SUCCSESS;
	Status res_run = SUCCSESS;
	Status res_delete = SUCCSESS;

	if (fncode & HIDDEN_UPLOAD)
	{
		while (std::getline(std::cin, line) && res_upload == SUCCSESS) // TODO change from getting one line at a time to one chunk at a time
		{
			res_upload = handel.putBytesInFile(stringParam, line);
		}
	}
	if ((fncode & HIDDEN_RUN) && res_upload == SUCCSESS) {
		res_run = handel.runFile(stringParam);
	}

	if ((fncode & HIDDEN_DELETE) && res_upload == SUCCSESS) {
		res_delete = handel.runFile(stringParam);
	}

	if (fncode & HIDDEN_LIST) {
		handel.listFiles();
	}

	return res_upload + (res_run << STATUS_SHIFT_AMOUT) + (res_delete << (2 * STATUS_SHIFT_AMOUT));
}

/*
int main(int argc, char** argv) {
	Hider hider = Hider();	
	return hider.manage_files(argc, argv);
}
*/
