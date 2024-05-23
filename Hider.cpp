#include "Hider.h"

#include "Errors.h"
#include "FunCodes.h"

Hider::Hider() : handel(HiddenFileHandler()) {}

Error Hider::manage_files(int argc, char* argv[])
{
	std::string line;
	if (argc < 2) {
		return HIDER_NO_ARGUMENTS_ERROR;
	}
	FunCode fncode = (FunCode)atoi(argv[1]);
	std::string stringParam = nullptr;

	if (argc >= 3) {
		std::string stringParam(argv[2]);
	}
	
	switch (fncode)
	{
	case HIDDEN_UPLOAD:
		while (std::getline(std::cin, line)) // TODO change from getting one line at a time to one chunk at a time
		{
			handel.putBytesInFile(stringParam, line);
		}
		break;

	case HIDDEN_DELETE:
		handel.removeFile(stringParam);
		break;

	case HIDDEN_RUN:
		handel.runFile(stringParam);
		break;

	case HIDDEN_LIST: // maybe will be removed
		handel.listFiles();
		break;

	default:
		return INVALID_FUNCODE_ERROR;
		break;
	}
	return SUCCSESS;
}

int main(int argc, char** argv) {
	Hider hider = Hider();	
	return hider.manage_files(argc, argv);
}