#include <iostream>
#include <stdint.h>

#include "encoding.cpp"



int main() {
    uint32_t testInt = 87;
    std::string enc = encodeInt(testInt);
    std::cout << "encrypted: " << enc << std::endl;
    uint32_t dec = decodeInt(enc);
    std::cout << "decrypted: " << dec << std::endl;

}