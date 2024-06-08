#include "Sniffer.h"
#include <thread>

using namespace std;

std::atomic<bool> Sniffer::stopSniffing = true;


// handler for packets. is sequential (so no locks yay!!)
bool Sniffer::callback(const Tins::PDU &pdu) {
    // Check if we should stop sniffing
    if (stopSniffing.load()) {
        throw std::runtime_error("Stopping sniffing");
    }

    // Find the IP layer
    const Tins::IP &ip = pdu.rfind_pdu<Tins::IP>(); 
    // Find the TCP layer
    const Tins::TCP &tcp = pdu.rfind_pdu<Tins::TCP>(); 

    std::string line = to_string(ip.size()) + " | " + 
            ip.src_addr().to_string() + ":" + to_string(tcp.sport()) + " -> " 
          + ip.dst_addr().to_string() + ":" + to_string(tcp.dport()) + "\n"; 

    if (strlen(buffer) + line.length() + 1 > FILE_SIZE)
        writeFile();

    strcat(buffer, line.c_str());

    return true;
}

// stop sniffing and write remaining data to file
int Sniffer::suicide() {
    raise(SIGINT);
    writeFile();
    return 0;
};


// Signal handler to handle SIGINT (Ctrl+C)
void Sniffer::signalHandler(int signal) {
    if (signal == SIGINT) {
        stopSniffing.store(true);
    }
}

// set up the sniffers loop and run
int Sniffer::run(const ContParams ContParams) {
    if (!stopSniffing.load()) // if already running no need to run() again
        return 1;
    std::cout << "-------- Starting to Sniff --------" << endl;
    stopSniffing.store(false);

    // Install signal handler for SIGINT
    std::signal(SIGINT, signalHandler);

    try {
        // Start sniffing loop
        Tins::Sniffer("ens33").sniff_loop([&](const Tins::PDU& pdu) {
            return this->callback(pdu);
        });
    } catch (const std::runtime_error& e) {
        std::cout << "-------- Sniffing stopped --------" << std::endl;
    }

    return 0;
}

// init sniffer object. not sniffing by default
Sniffer::Sniffer() {
    // init buffer to 0
    stopSniffing.store(true);
    memset(buffer, 0, FILE_SIZE);
    i = 0;
}


// writes buffer to file and zeroes buffer
int Sniffer::writeFile() {
    int res = Contraption::writeFile(to_string(i), buffer, strlen(buffer), OverWrite);
    memset(buffer, 0, FILE_SIZE);
    i++;
    return res;

}

// write remaining data
Sniffer::~Sniffer() {
    writeFile();
}
