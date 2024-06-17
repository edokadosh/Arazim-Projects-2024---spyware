#include "ContraptionAdmin.h"

const size_t TYPE2PARAMSIZE[] = { \
            [0] = 0, \
            [(uint32_t)KligerType] = sizeof(KligerParams), \
            [(uint32_t)SnifferType] = sizeof(SniffParams) \
        };


Status ContraptionAdmin::runContraption(Connection& conn, uint32_t paramsSize, contIdent_t identity)
{
    
    
    ContParams runParams;

    conn.recvData(paramsSize, (char*)&runParams);

    if (contMap.find(identity) != contMap.end())
    {
        return IDENTITY_ALREADY_TAKEN;
    }

    switch(runParams.type) {
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