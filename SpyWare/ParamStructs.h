#ifndef PARAM_STRUCTS_H
#define PARAM_STRUCTS_H

#define MAX_ADAPTER_NAMELEN (16)
#define MAX_DIR_NAMELEN (32)

typedef struct SniffParams {
    int time;
    char adapter[MAX_ADAPTER_NAMELEN];
} SniffParams;

#endif
