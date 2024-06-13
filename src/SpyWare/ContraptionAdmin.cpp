#include "ContraptionAdmin.h"

const size_t TYPE2PARAMSIZE[] = { \
            [0] = 0, \
            [(uint32_t)KligerType] = sizeof(KligerParams), \
            [(uint32_t)SnifferType] = sizeof(SniffParams) \
        };


Status ContraptionAdmin::runContraption(Connection& conn, ContType type, contIdent_t identity)
{
    
    const size_t paramsSize = TYPE2PARAMSIZE[type];
    
    ContParams runParams;

    runParams.type = type;
    conn.recvData(paramsSize, (char*)&runParams.parameters);

    if (contMap.find(identity) != contMap.end())
    {
        return IDENTITY_ALREADY_TAKEN;
    }

    switch(type) {
    case SnifferType:
        contMap[identity] = new Sniffer();
        break;
    case KligerType:
        // contMap[identity] = Kliger();
        break;
    }

    if (contMap[identity]->run(runParams) == 0) {
        return SUCCSESS;
    }

    return FAIL;
}

Status ContraptionAdmin::haltContraption(contIdent_t identity) {

    contMap[identity]->halt();
    delete contMap[identity];
    contMap.erase(identity);
    return SUCCSESS;
}