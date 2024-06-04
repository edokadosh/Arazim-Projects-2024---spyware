#include <iostream>
#include <tins/tins.h>

using namespace Tins;
using namespace std;

bool callback(const PDU &pdu) {
    // Find the IP layer
    const IP &ip = pdu.rfind_pdu<IP>(); 
    // Find the TCP layer
    const TCP &tcp = pdu.rfind_pdu<TCP>(); 

    cout << "size: " << ip.size() << " | ";
    cout << ip.src_addr() << ':' << tcp.sport() << " -> " 
         << ip.dst_addr() << ':' << tcp.dport() << endl;
    
    if (tcp.sport() == 80 || tcp.dport() == 80)
    {
        const RawPDU* pkt = pdu.find_pdu<RawPDU>();
        RawPDU::payload_type payload = pkt->payload();
        vector<uint8_t> dt = payload.data();
        cout << "HTTP: " <<  payload << endl;
    }

    return true;
}

int main() {
    Sniffer("ens33").sniff_loop(callback);
}