#include <stdint.h>
#include "encoding.h"

std::string encodeInt(uint32_t num) {
    uint8_t * arr = (uint8_t *)&num;
    const int len = 4;
    std::string encoded;
    bn::encode_b64(arr, arr + len, std::back_inserter(encoded));
    return encoded;
}

uint32_t decodeInt(std::string encoded) {
    uint8_t out[sizeof(uint32_t)]{ 0 };
    bn::decode_b64(encoded.begin(), encoded.end(), out);
    
    return *(uint32_t *)out;
}

std::string encodeStr(std::string pt) {
    const char* arr = pt.c_str();
    const int len = pt.length() / sizeof(char);
    std::string encoded;
    bn::encode_b64(arr, arr + len, std::back_inserter(encoded));
    return encoded;
}

std::string decodeStr(std::string ct) {
    std::string out;
    bn::decode_b64(ct.begin(), ct.end(), std::back_inserter(out));
    return out;
    
}

// TODO make encode struct function with template

// std::string encodeBytes(uint8_t* buffer, uint32_t len) {
//     std::string encoded;
//     bn::encode_b64(buffer, buffer + len, std::back_inserter(encoded));
//     return encoded;
// }

// void decodeBytes(uint8_t* buffer, uint32_t len) {
//     uint8_t out[sizeof(uint32_t)]{ 0 };
//     bn::decode_b64(encoded.begin(), encoded.end(), out);
    
//     return *(uint32_t *)out;
// }