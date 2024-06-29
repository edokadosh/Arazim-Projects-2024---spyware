#include <iostream>
#include <map>
#include <memory>

#include "Contraption.h"
#include "../IncludeCPP/Status.h"
#include "../Maneger/Connection.h"
#include "contrapMeta.h"
#include "ParamStructs.h"
#include "Sniffer.h"

#pragma once


class ContraptionAdmin {

private:
    std::map<uint32_t, Contraption *> contMap;

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