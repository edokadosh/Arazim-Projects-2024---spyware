#include "getBasicInfo.h"

// returns a unique machine ID
std::string getMachineID() {
    std::ifstream idFile("/etc/machine-id");

    if (!idFile.is_open()) {
        std::cerr << "error opening /etc/machine-id file\n";
        std::cerr << strerror(errno) << std::endl;
    }

    std::string id;
    std::getline(idFile, id);
    std::cerr << "machine-id: " << id << std::endl;
    return id;
}