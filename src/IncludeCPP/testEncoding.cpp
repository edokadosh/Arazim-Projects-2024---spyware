#include <iostream>
#include <stdint.h>

#include "encoding.cpp"



int main() {
    uint32_t testInt = 87;
    std::string enc = encodeInt(testInt);
    std::cout << "encrypted: " << enc << std::endl;
    uint32_t dec = decodeInt(enc);
    std::cout << "decrypted: " << dec << std::endl;

    std::string testString = "test string\n";
    std::string encS = encodeStr(testString);
    std::cout << "encrypted: " << encS << std::endl;
    std::string decS = decodeStr(encS);
    std::cout << "decrypted: " << decS << std::endl;

}