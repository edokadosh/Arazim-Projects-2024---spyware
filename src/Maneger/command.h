#include <cstdint>
#include <cstring>
#include <arpa/inet.h>

#include "FunCodes.h"
#include "../IncludeCPP/globalDefines.h"

#ifndef COMMAND_H
#define COMMAND_H

#define STRING_PARAM_LEN (100)



struct __attribute__((packed)) command {
    uint32_t dataLen;
    uint32_t fncode;
    uint32_t identifier;
    uint32_t writeMod;
    char strParam[STRING_PARAM_LEN];
};

typedef struct command command;

#endif