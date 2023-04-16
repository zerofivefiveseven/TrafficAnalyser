//
// Created by paul on 16.04.23.
//

#ifndef TRAFFICANALYSER_STATSHOLDER_H
#define TRAFFICANALYSER_STATSHOLDER_H

#include <cstdlib>
#include <queue>
#include <iostream>
#include <map>

namespace UserStructs {
    struct StatsHolderTmp;

    struct StatsHolderGlobal {
    protected:
        struct HostNameAssociated {
            std::string Hostname;
            unsigned Up;
            unsigned Down;
            unsigned UpPacketsCount;
            unsigned DownPacketsCount;
            HostNameAssociated& operator+=(const HostNameAssociated& rhs){

                this->Up+=rhs.Up;
                this->Down+=rhs.Down;
                this->UpPacketsCount+=rhs.UpPacketsCount;
                this->DownPacketsCount+=rhs.DownPacketsCount;
                return *this;
            }
        };

        std::map<std::string, HostNameAssociated> HostnamesStats;
    public:
        void printStats() const;

        //non-const extracting from Temp and insert to Glob
        //that's why we don't need to erase temp
        void MergeFromTmp(StatsHolderTmp &x);
    };


}


#endif //TRAFFICANALYSER_STATSHOLDER_H
