
#include "FunCodes.h"

#ifndef COMMAND_H
#define COMMAND_H

#define STRING_PARAM_LEN (50)



struct __attribute__((packed)) command {
    uint32_t dataLen;
    FunCode fncode;
    char strParam[STRING_PARAM_LEN];
};

typedef struct command command;

// create command in network endiness
command netEndianCommand(const command cmd) {
    command ans;
    ans.dataLen = htonl(cmd.dataLen);
    ans.fncode = (FunCode)htonl(cmd.fncode);
    memcpy(ans.strParam, cmd.strParam, sizeof(cmd.strParam));
    return ans;
}

command hostEndianCommand(const command cmd) {
    command ans;
    ans.dataLen = htonl(cmd.dataLen);
    ans.fncode = (FunCode)htonl(cmd.fncode);
    memcpy(ans.strParam, cmd.strParam, sizeof(cmd.strParam));
    return ans;
}

#endif