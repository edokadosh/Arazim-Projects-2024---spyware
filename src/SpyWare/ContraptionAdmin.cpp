#include "ContraptionAdmin.h"

const size_t TYPE2PARAMSIZE[] = { \
            [0] = 0, \
            [(uint32_t)KligerType] = sizeof(KligerParams), \
            [(uint32_t)SnifferType] = sizeof(SniffParams) \
        };

ContraptionAdmin::ContraptionAdmin() {
    std::cerr << "start constractor\n";
}


ContraptionAdmin::~ContraptionAdmin() {
    std::cerr << "contraption admin destractor" << std::endl;
}

Status ContraptionAdmin::runContraption(std::shared_ptr<Connection> conn, contIdent_t identity)
{
    ContParams runParams;

    std::cerr << "map print: ";
    write(STDERR_FILENO, &contMap, sizeof(contMap));
    std::cerr << std::endl;

    conn->recvData(sizeof(runParams), (char*)&runParams);
    std::cerr << "recved contraption parameters" << std::endl;

    if (contMap.find(identity) != contMap.end())
    {
        return IDENTITY_ALREADY_TAKEN;
    }
    std::cerr << "didn't find cont id in map" << std::endl;
    Contraption * cont = nullptr;
    switch(runParams.type) {
    case SnifferType:
        std::cerr << "tring to create a sniffer" << std::endl;
        cont = new Sniffer();
        std::cerr << "created sniffer" << std::endl;
        std::cerr << "id:" << identity << std::endl;
        std::cerr << "map size: " << contMap.size() << std::endl;

        contMap.insert({(uint32_t)identity, cont});
        std::cerr << "map insertion didn't segment fault" << std::endl;

        break;

        default:
            return INVALID_RUN_PARAMS_TYPE;
    }

    std::cerr << "start running contraption" << std::endl;
    std::cerr << "runParams time: " << runParams.parameters.sniffP.time << std::endl;

    return runContBackgrnd(identity, runParams);
}

Status ContraptionAdmin::runContBackgrnd(contIdent_t contId, const ContParams& contParams) {
    if (contMap.find(contId) == contMap.end()) {
        return CONTRAPTION_NOT_FOUND;
    }
    threadMap.insert({(uint32_t)contId, std::thread(&Contraption::run, contParams)});
}

Status ContraptionAdmin::haltContraption(contIdent_t identity) {
    contMap[identity]->halt();
    threadMap[identity].join();
    delete contMap[identity];
    contMap.erase(identity);
    return SUCCSESS;
}