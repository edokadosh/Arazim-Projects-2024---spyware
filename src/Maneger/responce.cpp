#include "responce.h"

// create responce in network endiness
responce netEndianResponce(const responce res) {
    // return (responce){.dataLen = htonl(res.dataLen), .status = htonl(res.status)};
    return res;
}

responce hostEndianResponce(const responce res) {
    // return (responce){.dataLen = ntohl(res.dataLen), .status = ntohl(res.status)};
    return res;
}