#include "SpyWare.h"

int main() {
    Sniffer s = Sniffer();
    ContParams c;
    c.parameters.snifP.time = 5;
    strncpy(c.parameters.snifP.adapter, "ens33", MAX_ADAPTER_NAMELEN);
    c.parameters.snifP.adapter[MAX_ADAPTER_NAMELEN - 1] = 0;
    c.type = SnifferType;

    s.run(c);

    sleep(10);
    s.run(c);

    return 0;
    // now destructor will be called
    // this will invoke writeFile to save last received data
}