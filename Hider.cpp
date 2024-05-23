#include "Hider.h"
Hider::Hider(std::string nameDirectory) : handel(FileHandler(nameDirectory)) {}
int Hider::manage_files() {
	int prosses=-1;
	std::string nameFlie;
	std::string line;
	std::cin >> prosses;
	switch (prosses)
	{
	case 0:
		std::cin >> nameFlie;
		
		while (std::getline(std::cin, line))
		{
			std::cout << line << std::endl;
			handel.putBytesInFile(nameFlie, line);
		}
		break;
	case 1:
		std::cin >> nameFlie;
		handel.removeFile(nameFlie);
		break;
	case 2:
		std::cin >> nameFlie;
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
	Hider hider = Hider(std::string(argv[0]));
	while (true) {
		hider.manage_files();
	}

}