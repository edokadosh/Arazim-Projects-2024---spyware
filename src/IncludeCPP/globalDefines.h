#ifndef GLOBAL_DEFINES_H
#define GLOBAL_DEFINES_H

#define CHUNK_SIZE (1024)
#define MIN(x, y) (((x) < (y)) ? (x): (y))

#define PAGE_SIZE (4096)

#define HOME_HOST ("localhost")

typedef enum WriteMod {
    OverWrite = 0,
    AppendMod,
} WriteMod;

#endif