//
// Created by paul on 16.04.23.
//

#ifndef TRAFFICANALYSER_STATSHOLDER_H
#define TRAFFICANALYSER_STATSHOLDER_H

#include <cstdlib>
#include <iostream>
#include <map>
#include <optional>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <gtest/gtest.h>
namespace UserStructs {
    struct StatsHolderTmp;

    struct StatsHolderResult {             /* :public ::testing::Test  public ::testing::Test error: cannot declare variable ‘StatsInstanceTemp’ to be of abstract type ‘UserStructs::StatsHolderTmp’
  159 |     UserStructs::StatsHolderTmp StatsInstanceTemp;*/

    public:
        struct HostNameAssociated {
            std::string IpAddr;
            std::optional<std::string> oHostname;
            unsigned Up;
            unsigned Down;
            unsigned UpPacketsCount;
            unsigned DownPacketsCount;

            HostNameAssociated &operator+=(const HostNameAssociated &rhs) {
                if (!this->oHostname.has_value()) {
                    BOOST_LOG_TRIVIAL(debug)
                        << "Hostname was not provided in thus node but added by request packet via+= opertaor: "
                        << rhs.oHostname.value();
                    this->oHostname = rhs.oHostname.value();
                }
                this->Up += rhs.Up;
                this->Down += rhs.Down;
                this->UpPacketsCount += rhs.UpPacketsCount;
                this->DownPacketsCount += rhs.DownPacketsCount;
                return *this;
            }
        };


        void printStats() const;

        void MergeFromTmp(StatsHolderTmp &x);

    protected:
        std::map<std::string, HostNameAssociated> HostnamesStats;
    };


}


#endif //TRAFFICANALYSER_STATSHOLDER_H
