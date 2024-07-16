#ifndef GLOBAL_DEFINES_H
#define GLOBAL_DEFINES_H

#define CHUNK_SIZE (1024)

#ifndef MIN
#define MIN(x, y) (((x) < (y)) ? (x): (y))
#endif

#define PAGE_SIZE (4096)

// #define HOME_HOST ("localhost")
//#pragma warning "notice that it is defined that the server is on avner's wsl"
//#define HOME_HOST ("172.26.222.10") // avner's wsl
// #define HOME_HOST ("192.168.120.10")
// #define HOME_HOST ("192.168.115.1") // avner

#define HOME_HOST ("192.168.120.218")



#define M_OVERWRITE (1)
#define M_APPEND (2)

#endif