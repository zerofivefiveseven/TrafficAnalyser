//
// Created by paul on 16.04.23.
//

#ifndef TRAFFICANALYSER_STATSHOLDER_H
#define TRAFFICANALYSER_STATSHOLDER_H

#include <cstdlib>
#include <iostream>
#include <map>
#include <optional>

namespace UserStructs {
    struct StatsHolderTmp;

    struct StatsHolderGlobal {

    public:
        struct HostNameAssociated {
            std::string IpAddr;
            std::optional<std::string> oHostname;
            unsigned Up;
            unsigned Down;
            unsigned UpPacketsCount;
            unsigned DownPacketsCount;
            HostNameAssociated& operator+=(const HostNameAssociated& rhs){
                if(!this->oHostname.has_value()){
                    this->oHostname = rhs.oHostname.value();
                }
                this->Up+=rhs.Up;
                this->Down+=rhs.Down;
                this->UpPacketsCount+=rhs.UpPacketsCount;
                this->DownPacketsCount+=rhs.DownPacketsCount;
                return *this;
            }
        };


        void printStats() const;

        //non-const extracting from Temp and insert to Glob
        //that's why we don't need to erase temp
        void MergeFromTmp(StatsHolderTmp &x);

    protected:
        std::map<std::string, HostNameAssociated> HostnamesStats;
    };


}


#endif //TRAFFICANALYSER_STATSHOLDER_H
