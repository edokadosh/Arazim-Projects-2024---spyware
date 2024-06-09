

#ifndef CONTRAP_META_H
#define CONTRAP_META_H

typedef enum WriteMod {
    OverWrite = 0,
    AppendMod,
} WriteMod;

typedef union Params {


} Params;

typedef enum ContType {
    Kliger = 0,
} ContType;

typedef __attribute__((packed)) struct ContParams {
    ContType type;
    Params parameters;
} ContParams;

typedef enum SpywareCmdType {

    RunContraption = 1,
    HaltContraption = 2,
} SpywareCmdType;


typedef __attribute__((packed)) struct SpywareCmd {
    SpywareCmdType cmdType;
    uint32_t cmdLen;
} SpywareCommand;

#endif