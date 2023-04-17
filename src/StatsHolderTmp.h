//
// Created by paul on 16.04.23.
//

#ifndef ANALYSER_STATSHOLDERTMP_H
#define ANALYSER_STATSHOLDERTMP_H

#include "StatsHolderResult.h"

namespace UserStructs {
    struct StatsHolderTmp : StatsHolderResult {
        friend StatsHolderResult;
    public:
        void Update(const HostNameAssociated &);

    };

}


#endif //ANALYSER_STATSHOLDERTMP_H
