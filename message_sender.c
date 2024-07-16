#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include "message_slot.h"

int main(int argc, char* argv[]) {

    sleep(3);
    int fd = -1;

    if (argc != 4) {
        fprintf(stderr, "invalid amount of arguments\n");
        return EXIT_FAILURE;
    }

    int len = strlen(argv[3]);
    if (len > MAX_MSG_SIZE || len <= 0) {
        fprintf(stderr, "invalid mssage legnth\n");
        return EXIT_FAILURE;
    }

    fprintf(stdout, "start open device\n");

    if ((fd = open(argv[1], O_WRONLY)) == -1) {
        fprintf(stderr, "error opening device: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    fprintf(stdout, "open device succsess\n");


    if (ioctl(fd, MSG_SLOT_CHANNEL, atoi(argv[2])) == -1) {
        fprintf(stderr, "error doing ioctl to device: %s\n", strerror(errno));
        close(fd);
        return EXIT_FAILURE;
    }
    fprintf(stdout, "ioctl device succsess\n");

    if (write(fd, argv[3], len) < 0) {
        fprintf(stderr, "error to writing to device: %s\n", strerror(errno));
        close(fd);
        return EXIT_FAILURE;
    }
    fprintf(stdout, "write device succsess\n");

    close(fd);

    return EXIT_SUCCESS;
}

