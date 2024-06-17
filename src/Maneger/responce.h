#include <arpa/inet.h>
#include "../IncludeCPP/Status.h"


#ifndef RESPONCE_H
#define RESPONCE_H

// meant to be in network endiness
struct __attribute__((packed)) responce {
    uint32_t dataLen;
    uint32_t status;
};

typedef struct responce responce;


#endif