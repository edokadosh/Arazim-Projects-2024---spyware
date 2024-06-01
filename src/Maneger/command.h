
#include <cstdint>
#include <cstring>
#include <arpa/inet.h>

#include "FunCodes.h"

#ifndef COMMAND_H
#define COMMAND_H

#define STRING_PARAM_LEN (50)



struct __attribute__((packed)) command {
    uint32_t dataLen;
    uint32_t fncode;
    char strParam[STRING_PARAM_LEN];
};

typedef struct command command;

command netEndianCommand(const command cmd);

command hostEndianCommand(const command cmd);

#endif