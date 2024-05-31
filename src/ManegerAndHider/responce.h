#include <arpa/inet.h>
#include "Status.h"


#ifndef RESPONCE_H
#define RESPONCE_H

// meant to be in network endiness
struct __attribute__((packed)) responce {
    uint32_t dataLen;
    uint32_t status;
};

typedef struct responce responce;

responce netEndianResponce(const responce res);

responce hostEndianResponce(const responce res);

#endif