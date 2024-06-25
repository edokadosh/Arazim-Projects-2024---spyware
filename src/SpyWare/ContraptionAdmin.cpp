#include "ContraptionAdmin.h"

const size_t TYPE2PARAMSIZE[] = { \
            [0] = 0, \
            [(uint32_t)KligerType] = sizeof(KligerParams), \
            [(uint32_t)SnifferType] = sizeof(SniffParams) \
        };

ContraptionAdmin::ContraptionAdmin() {
    std::cerr << "start constractor\n";
    // segTest();
}

// void ContraptionAdmin::segTest() {
//     static uint32_t i;
//     contMap.insert({i, i});
//     for(auto it = contMap.cbegin(); it != contMap.cend(); ++it)
//     {
//         std::cerr << "(" << it->first << ", " << it->second << ") ";
//     }
//     std::cerr << std::endl;
//     i++;
// }

ContraptionAdmin::~ContraptionAdmin() {
    std::cerr << "contraption admin destractor" << std::endl;
}

Status ContraptionAdmin::runContraption(std::shared_ptr<Connection> conn, contIdent_t identity)
{
    ContParams runParams;
    // segTest();

    std::cerr << "map print: ";
    write(STDERR_FILENO, &contMap, sizeof(contMap));
    std::cerr << std::endl;
    // segTest();

    conn->recvData(sizeof(runParams), (char*)&runParams);
    std::cerr << "recved contraption parameters" << std::endl;
    // segTest();

    if (contMap.find(identity) != contMap.end())
    {
        return IDENTITY_ALREADY_TAKEN;
    }
    // segTest();
    std::cerr << "didn't find cont id in map" << std::endl;
    Contraption * cont = nullptr;
    switch(runParams.type) {
    case SnifferType:
        // segTest();
        std::cerr << "tring to create a sniffer" << std::endl;
        cont = new Sniffer();
        std::cerr << "created sniffer" << std::endl;
        std::cerr << "id:" << identity << std::endl;
        std::cerr << "map size: " << contMap.size() << std::endl;

        contMap.insert({(uint32_t)identity, cont});
        // segTest();
        std::cerr << "map insertion didn't segment fault" << std::endl;

        break;

        default:
            return INVALID_RUN_PARAMS_TYPE;
    }


    std::cerr << "start running contraption" << std::endl;
    std::cerr << "runParams time: " << runParams.parameters.sniffP.time << std::endl;
    if (contMap[identity]->run(runParams) == 0) {
        return SUCCSESS;
    }

    return FAIL;
}

Status ContraptionAdmin::haltContraption(contIdent_t identity) {

    // contMap[identity]->halt();
    // delete contMap[identity];
    contMap.erase(identity);
    return SUCCSESS;
}