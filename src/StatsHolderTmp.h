//
// Created by paul on 16.04.23.
//

#ifndef ANALYSER_STATSHOLDERTMP_H
#define ANALYSER_STATSHOLDERTMP_H

#include "StatsHolderGlobal.h"

namespace UserStructs {
    struct StatsHolderTmp : StatsHolderGlobal {
        friend StatsHolderGlobal;
    public:
        void Update(const HostNameAssociated &);

    };

}


#endif //ANALYSER_STATSHOLDERTMP_H
