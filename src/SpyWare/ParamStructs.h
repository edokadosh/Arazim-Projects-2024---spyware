#ifndef PARAM_STRUCTS_H
#define PARAM_STRUCTS_H

#define MAX_ADAPTER_NAMELEN (16)

typedef struct SniffParams {

    int time;
    char adapter[MAX_ADAPTER_NAMELEN];

} SniffParams;

// TODO create this struct
typedef struct KligerParams {

    int time;

} KligerParams;

#endif
