#include <iostream>

#include "Errors.h"
#include "SoftwareManeger.h"

void testSoftwareManeger(void);

int main(int argc, char * argv[]) {
    testSoftwareManeger();
    return 0;
}

void testSoftwareManeger() {
    SoftwareManeger swm = SoftwareManeger();
    swm.writeFile("tempSwmTest.txt", false, "this is my first test\n");
    swm.writeFile("tempSwmTest.txt", 1, "this is my second test\n");
    
    swm.deleteFile("tempSwmTest.txt");

}
