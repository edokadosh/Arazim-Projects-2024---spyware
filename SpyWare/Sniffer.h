#include <iostream>
#include <tins/tins.h>
#include <csignal> // for signal handling
#include <atomic> // for atomic flag
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

    // Global atomic flag to control sniffing loop
    static std::atomic<bool> stopSniffing;

public:
    Sniffer(/* args */);
    ~Sniffer();

    int suicide() override;
    int run(const ContParams ContParams) override;
    
};


/*
USAGE EXAMPLE:

int main() {
    Sniffer s = Sniffer();
    ContParams c;

    std::thread thread([&]() { s.run(c); });   
    sleep(5);
    s.suicide();
    thread.join();
    std::cout << "suicided. Now restarting" << endl;

    std::thread thread2([&]() { s.run(c); });   
    sleep(5);
    s.suicide();
    thread2.join();

    // now destructor will be called
    // this will invoke writeFile to save last received data
}

*/
