#include <stdint.h>
#include <iostream>
#include <string>

typedef enum WriteMod {
    OverWrite = 0,
    AppendMod,
} WriteMod;

union Params {


} Params;

typedef enum ContType {
    Kliger = 0,
} ContType;

typedef struct ContParams {
    ContType type;
} ContParams;


class Contraption {

private:
    
public:

    int readFile(const std::string& fileName, char buffer[], uint32_t maxLen);

    int writeFile(const std::string& fileName, char buffer[], uint32_t len, WriteMod writeMod);

    virtual int suicide();

    virtual int run(const ContParams);

};