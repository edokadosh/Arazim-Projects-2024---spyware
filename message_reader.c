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
    int amountRead = 0;

    char msg[MAX_MSG_SIZE] = { 0 };

    if (argc != 3) {
        fprintf(stderr, "invalid amount of arguments\n");
        return EXIT_FAILURE;
    }

    fprintf(stdout, "start open device\n");
    if ((fd = open(argv[1], O_RDONLY)) == -1) {
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


    if ((amountRead = read(fd, msg, sizeof(msg))) < 0) {
        fprintf(stderr, "error reading from device: %s\n", strerror(errno));
        close(fd);
        return EXIT_FAILURE;
    }
    fprintf(stdout, "read device succsess\n");


    if(write(STDOUT_FILENO, msg, amountRead) < 0) {
        fprintf(stderr, "error writing mssage to stdout: %s\n", strerror(errno));
        close(fd);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}