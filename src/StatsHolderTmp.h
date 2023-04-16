//
// Created by paul on 16.04.23.
//

#ifndef ANALYSER_STATSHOLDERTMP_H
#define ANALYSER_STATSHOLDERTMP_H

#include <cstdlib>
#include <queue>
#include <map>
#include <iostream>
#include "StatsHolderGlobal.h"

namespace UserStructs {
    struct StatsHolderTmp : StatsHolderGlobal {
        friend StatsHolderGlobal;
    private:
        std::queue<std::pair<std::string, unsigned>> requestQue;
    public:
        void UpdateByResponse(unsigned);

        void addRequestToQue(const std::pair<std::string, unsigned> &);

    };

}


#endif //ANALYSER_STATSHOLDERTMP_H
