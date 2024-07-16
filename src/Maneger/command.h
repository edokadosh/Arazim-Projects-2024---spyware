#include <cstdint>
#include <cstring>
#include <arpa/inet.h>

#include "FunCodes.h"

#ifndef COMMAND_H
#define COMMAND_H

#define STRING_PARAM_LEN (200)



struct __attribute__((packed)) command {
    uint32_t dataLen;
    uint32_t fncode;
    uint32_t identifier;
    char strParam[STRING_PARAM_LEN];
};

typedef struct command command;

#endif