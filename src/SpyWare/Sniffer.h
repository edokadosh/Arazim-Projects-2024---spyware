#pragma once

#include <iostream>
#include <tins/tins.h>
#include <csignal> // for signal handling
#include <atomic> // for atomic flag
#include <thread>
#include <vector>
#include <condition_variable>
#include <mutex>

#include "Contraption.h"

#define FILE_SIZE 4096

// COMPILATION FLAG NEEDED: -ltins
// USAGE EXAMPLE: g++ -o Sniffer Sniffer.cpp -ltins

class Sniffer : public Contraption
{
private:
    int i;
    char buffer[FILE_SIZE];
    
    bool callback(const Tins::PDU &pdu);
    int writeFile();
    static void signalHandler(int signal);
    std::vector<std::thread> threads;
    std::condition_variable cv;
    std::mutex mtx;
    bool threadStarting;

    void runTime(SniffParams sniffParams, int t);
    int sniff(const SniffParams sniffParams);

    // Global atomic flag to control sniffing loop
    static std::atomic<bool> stopSniffing;

public:
    Sniffer(/* args */);
    ~Sniffer();

    int halt() override;
    int run(const ContParams sniffParams) override;
};


/*
USAGE EXAMPLE:

int main() {
    Sniffer s = Sniffer();
    ContParams c;
    c.parameters.snifP.time = 5;
    strncpy(c.parameters.snifP.adapter, "ens33", MAX_ADAPTER_NAMELEN);
    c.parameters.snifP.adapter[MAX_ADAPTER_NAMELEN - 1] = 0;
    c.type = SnifferType;

    s.run(c);

    sleep(10); // wait to be finisished
    s.run(c);

    return 0;
    // now destructor will be called
    // this will invoke writeFile to save last received data
}

*/
