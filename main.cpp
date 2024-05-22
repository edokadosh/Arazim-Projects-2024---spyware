#include <iostream>

#include "Errors.h"
#include "client.h"
#include "SoftwareManeger.h"

void testSoftwareManeger(void);

int main(int argc, char * argv[]) {
    
    Client client;
    client.createSocket();
    SoftwareManeger swm = SoftwareManeger();

    while (true)
    {
        loopIter(client, swm);
    }

    return 0;
}

void testSoftwareManeger() {
    SoftwareManeger swm = SoftwareManeger();
    swm.writeFile("tempSwmTest.txt", false, "this is my first test\n");
    swm.writeFile("tempSwmTest.txt", 1, "this is my second test\n");
    
    swm.deleteFile("tempSwmTest.txt");

}

void loopIter(Client& client, SoftwareManeger& swm)
{
    // receive command
    Message msg;
    client.recvCommand(msg);
    int fnccode = msg.fnccode();
    std::string command = msg.param1();

    std::string response = "";
    switch (fnccode)
    {
    case /* constant-expression */:
        /* code */
        break;
    
    default:
        break;
    }
    
    // return answer
}
