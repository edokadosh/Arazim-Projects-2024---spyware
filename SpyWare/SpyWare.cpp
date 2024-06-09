#include "SpyWare.h"

int main() {
    Sniffer s = Sniffer();
    ContParams c;
    c.parameters.snifP.time = 30;
    c.type = SnifferType;

    s.run(c);

    return 0;
    // now destructor will be called
    // this will invoke writeFile to save last received data
}