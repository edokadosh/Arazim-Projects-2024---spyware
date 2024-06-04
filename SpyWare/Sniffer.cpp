#include <iostream>
#include <tins/tins.h>

#define SOCK_PATH    "/home/sivan/cyber/echo_socket"

using namespace Tins;
using namespace std;

bool callback(const PDU &pdu) {
    // Find the IP layer
    const IP &ip = pdu.rfind_pdu<IP>(); 
    // Find the TCP layer
    const TCP &tcp = pdu.rfind_pdu<TCP>(); 
    cout << ip.src_addr() << ':' << tcp.sport() << " -> " 
         << ip.dst_addr() << ':' << tcp.dport() << endl;
    return true;
}

int main() {
    Sniffer("eth0").sniff_loop(callback);
}