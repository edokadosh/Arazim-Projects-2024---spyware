

#ifndef CONTRAP_META_H
#define CONTRAP_META_H

typedef enum WriteMod {
    OverWrite = 0,
    AppendMod,
} WriteMod;

union Params {


} Params;

typedef enum ContType {
    Kliger = 0,
} ContType;

typedef struct ContParams {
    ContType type;
} ContParams;

#endif