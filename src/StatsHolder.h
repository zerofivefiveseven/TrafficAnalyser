//
// Created by paul on 16.04.23.
//

#ifndef TRAFFICANALYSER_STATSHOLDER_H
#define TRAFFICANALYSER_STATSHOLDER_H
#include <cstdlib>
#include <queue>
#include <map>
#include <iostream>

namespace UserStructs {
    struct StatsHolderTmp;

    struct StatsHolderGlobal {
    protected:
        std::map<std::string, std::pair<long, long>> HostnamesStats;
    public:
        void printStats() const;

        //non-const extracting from Temp and insert to Glob
        //that's why we don't need to erase temp
        void MergeFromTmp(StatsHolderTmp &);
    };


    struct StatsHolderTmp : StatsHolderGlobal {
        friend StatsHolderGlobal;
    private:
        std::queue<std::pair<std::string, int>> requestQue;
    public:
        void UpdateByResponse(int);
        void addRequestToQue(const std::pair<std::string, int> &);

    };


}


#endif //TRAFFICANALYSER_STATSHOLDER_H
