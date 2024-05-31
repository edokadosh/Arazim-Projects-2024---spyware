#include "Status.h"
#include <arpa/inet.h>

#ifndef RESPONCE_H
#define RESPONCE_H

// meant to be in network endiness
struct __attribute__((packed)) responce {
    uint32_t dataLen;
    uint32_t status;
};

typedef struct responce responce;

// create responce in network endiness
responce netEndianResponce(const responce res) {
    return (responce){.dataLen = htonl(res.dataLen), .status = htonl(res.status)};
}

responce hostEndianResponce(const responce res) {
    return (responce){.dataLen = ntohl(res.dataLen), .status = ntohl(res.status)};
}

#endif