#include "ContraptionAdmin.h"

const size_t TYPE2PARAMSIZE[] = { \
            [0] = 0, \
            [(uint32_t)KligerType] = sizeof(KligerParams), \
            [(uint32_t)SnifferType] = sizeof(SniffParams), \
            [(uint32_t)BuggType] = sizeof(BuggParams), \
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


    conn->recvData(sizeof(runParams), (char*)&runParams);
    std::cerr << "recved contraption parameters" << std::endl;

    if (contMap.find(identity) != contMap.end())
    {
        return IDENTITY_ALREADY_TAKEN;
    }
    std::cerr << "didn't find cont id in map" << std::endl;
    std::shared_ptr<Contraption> cont;
    std::cerr << "runParams.type=" << runParams.type << std::endl;
    switch(runParams.type) {
    case SnifferType:
        std::cerr << "tring to create a sniffer" << std::endl;
        cont = std::make_shared<Sniffer>();
        std::cerr << "created sniffer" << std::endl;
        std::cerr << "id:" << identity << std::endl;

        // std::cerr << "map insertion didn't segment fault" << std::endl;

        break;
    case KligerType:
        std::cerr << "tring to create a kligger" << std::endl;
        cont = std::make_shared<Kligger>();
        std::cerr << "created kligger" << std::endl;
        std::cerr << "id:" << identity << std::endl;
        break;
    
    case BuggType:
        std::cerr << "tring to create a Bugg" << std::endl;
        cont = std::make_shared<Bugg>();
        std::cerr << "created Bugg" << std::endl;
        std::cerr << "id:" << identity << std::endl;
        break;

        default:
            return INVALID_RUN_PARAMS_TYPE;
    }
    
    contMap.insert({(uint32_t)identity, cont});

    std::cerr << "start running contraption" << std::endl;
    // std::cerr << "runParams time: " << runParams.parameters.sniffP.time << std::endl;

    return runContBackgrnd(identity, runParams);
}

Status ContraptionAdmin::runContBackgrnd(contIdent_t contId, const ContParams& contParams) {
    auto cont = contMap.find(contId);
    if (cont == contMap.end()) {
        return CONTRAPTION_NOT_FOUND;
    }
    auto contThread = std::make_shared<std::thread>(&Contraption::run, cont->second, contParams);
    
    threadMap.emplace((uint32_t)contId, contThread); // BUG THIS LINE NOT WORKING
    return SUCCSESS;
}

Status ContraptionAdmin::haltContraption(contIdent_t identity) {
    contMap[identity]->halt();
    threadMap[identity]->join();
    // delete contMap[identity];
    contMap.erase(identity);
    return SUCCSESS;
}