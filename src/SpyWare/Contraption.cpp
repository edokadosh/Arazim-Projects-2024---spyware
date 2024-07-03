#include "Contraption.h"

int Contraption::writeFile(const std::string& fileName, char buffer[], uint32_t len, uint32_t writeMod) 
{
    if (writeMod != OverWrite)
        return 1; // not implemented sorry ):

    int bufferLen = strlen(buffer);
    if (bufferLen == 0)
        return 0;

    std::cerr << "Writing to file " << fileName << std::endl;
    std::ofstream outputFile(fileName);

    if (outputFile.is_open()) {
        outputFile.write(buffer, std::min((unsigned int)len, (unsigned int)bufferLen + 1));
        outputFile.close();
    } else {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
    }

    return 0;
}
