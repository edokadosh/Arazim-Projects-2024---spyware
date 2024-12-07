#include <iostream>

#include "basen.hpp"

#ifndef ENCODING_H
#define ENCODING_H

std::string encodeInt(uint32_t num);

uint32_t decodeInt(std::string encoded);

std::string encodeStr(std::string pt);

std::string decodeStr(std::string ct);

#endif
