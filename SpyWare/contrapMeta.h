
#include "ParamStructs.h"

#ifndef CONTRAP_META_H
#define CONTRAP_META_H

typedef enum WriteMod {
    OverWrite = 0,
    AppendMod,
} WriteMod;

typedef union Params {
    SniffParams snifP;

} Params;

typedef enum ContType {
    KligerType = 1,
    SnifferType = 2,
} ContType;

typedef struct ContParams {
    ContType type;
    Params parameters;
} ContParams;

#endif