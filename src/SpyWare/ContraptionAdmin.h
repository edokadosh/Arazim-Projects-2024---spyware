#include <iostream>
#include <map>
#include <memory>

#include "Contraption.h"
#include "../IncludeCPP/Status.h"
#include "../Maneger/Connection.h"
#include "contrapMeta.h"
#include "ParamStructs.h"
#include "Sniffer.h"
#include "kligger.h"

#pragma once


class ContraptionAdmin {

private:
    std::map<uint32_t, std::shared_ptr<Contraption>> contMap;
    std::map<uint32_t, std::shared_ptr<std::thread>> threadMap;

    Status runContBackgrnd(contIdent_t contId, const ContParams& contParams);

public:

    ContraptionAdmin();

    ~ContraptionAdmin();

    // ~ContraptionAdmin() {
    //     contMap.clear();
    // }

    Status runContraption(std::shared_ptr<Connection> conn, contIdent_t identity);

    Status haltContraption(contIdent_t identity);

    Status listContraptions(std::shared_ptr<Connection> conn);
};