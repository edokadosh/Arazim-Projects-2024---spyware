#include "Hider.h"
Hider::Hider() : handel(HiddenFileHandler()) {}
int Hider::manage_files(int prosses, std::string nameFlie) {
	std::string line;
	switch (prosses)
	{
	case 0:
		while (std::getline(std::cin, line))
		{
			std::cout << line << std::endl;
			handel.putBytesInFile(nameFlie, line);
		}
		break;
	case 1:
		handel.removeFile(nameFlie);
		break;
	case 2:
		handel.runFile(nameFlie);
		break;
	case 3:
		handel.listFiles();
		break;
	default:
		return 1;
		break;
	}
	return 0;
}
int main(int argc, char** argv) {
	Hider hider = Hider();
	while (true) {
		hider.manage_files((int)argv[0],(std::string)argv[1]);
	}
	return 0;
}