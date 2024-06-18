#include <iostream>
#include <map>

#include "Contraption.h"
#include "../IncludeCPP/Status.h"
#include "../Maneger/Connection.h"
#include "contrapMeta.h"
#include "ParamStructs.h"
#include "Sniffer.h"

#pragma once


class ContraptionAdmin {

private:
    std::map<contIdent_t, Contraption *> contMap;

public:

    ContraptionAdmin() {};

    ~ContraptionAdmin() {
        contMap.clear();
    };

    Status runContraption(Connection& conn, contIdent_t identity);

    Status haltContraption(contIdent_t identity);
};