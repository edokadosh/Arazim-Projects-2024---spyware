#ifndef PARAM_STRUCTS_H
#define PARAM_STRUCTS_H

#define MAX_ADAPTER_NAMELEN (16)

#pragma pack(1)
struct SniffParams {

    int time;
    char adapter[MAX_ADAPTER_NAMELEN];

};
typedef struct SniffParams SniffParams;


// TODO create this struct
#pragma pack(1)
struct KligerParams {

    int time;
};
typedef struct KligerParams KligerParams;

#endif
