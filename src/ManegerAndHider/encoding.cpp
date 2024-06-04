#include <stdint.h>
#include "encoding.h"

std::string encodeInt(uint32_t num) {
    uint8_t * arr = (uint8_t *)&num;
    const int len = 4;
    std::string encoded;
    bn::encode_b64(arr, arr + len, std::back_inserter(encoded));
    return encoded;

    uint8_t out[len]{ 0 };
    bn::decode_b64(encoded.begin(), encoded.end(), out);
    
}

int32_t decodeInt(std::string encoded) {
    const int len = 4;
    uint8_t out[len]{ 0 };
    bn::decode_b64(encoded.begin(), encoded.end(), out);
    
    return *(int32_t *)out;
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
    bn::encode_b64(ct.begin(), ct.end(), std::back_inserter(out));
    return out;
    
}
