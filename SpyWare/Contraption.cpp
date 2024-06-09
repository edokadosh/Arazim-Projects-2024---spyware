#include "Contraption.h"

int Contraption::writeFile(const std::string& fileName, char buffer[], uint32_t len, WriteMod writeMod) 
{
    if (writeMod != OverWrite)
        return 1; // not implemented sorry ):
    std::cout << "Writing to file " << fileName << std::endl;
    std::ofstream outputFile(fileName);

    if (outputFile.is_open()) {
        outputFile.write(buffer, std::min((unsigned int)len, (unsigned int)strlen(buffer) + 1));
        outputFile.close();
    } else {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
    }

    return 0;
}
