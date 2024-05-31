
#include "FunCodes.h"

#ifndef COMMAND_H
#define COMMAND_H

#define STRING_PARAM_LEN (50)

struct __attribute__((packed)) command {
    FunCode fncode;
    int dataLen;
    char strParam[STRING_PARAM_LEN];
};

typedef struct command command;

#endif