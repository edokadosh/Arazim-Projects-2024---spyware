#include <iostream>
#include <tins/tins.h>

using namespace Tins;
using namespace std;

/*

Tasks:
locks
make into class
make basic version

*/

bool callback(const PDU &pdu) {
    // Find the IP layer
    const IP &ip = pdu.rfind_pdu<IP>(); 
    // Find the TCP layer
    const TCP &tcp = pdu.rfind_pdu<TCP>(); 

    cout << "size: " << ip.size() << " | ";
    cout << ip.src_addr() << ':' << tcp.sport() << " -> " 
         << ip.dst_addr() << ':' << tcp.dport() << endl;

    return true;
}

int main() {
    Sniffer("ens33").sniff_loop(callback);
}