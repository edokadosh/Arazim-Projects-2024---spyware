#include "command.h"


// create command in network endiness
command netEndianCommand(const command cmd) {
    command ans;
    ans.dataLen = htonl(cmd.dataLen);
    ans.fncode = htonl(cmd.fncode);
    memcpy(ans.strParam, cmd.strParam, sizeof(cmd.strParam));
    return ans;
}

command hostEndianCommand(const command cmd) {
    command ans;
    ans.dataLen = htonl(cmd.dataLen);
    ans.fncode = htonl(cmd.fncode);
    memcpy(ans.strParam, cmd.strParam, sizeof(cmd.strParam));
    return ans;
}