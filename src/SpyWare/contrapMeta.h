#include "ParamStructs.h"


#ifndef CONTRAP_META_H
#define CONTRAP_META_H

typedef uint32_t contIdent_t;


typedef union Params {
    SniffParams sniffP;
} Params;

typedef enum ContType : uint32_t {
    KligerType = 1,
    SnifferType = 2,
} ContType;

typedef struct __attribute__((packed)) ContParams {
    ContType type;
    Params parameters;
} ContParams;



#endif
