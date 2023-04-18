//
// Created by paul on 16.04.23.
//
//Стоило наверное делать абстрактный класс
#include "StatsHolderResult.h"
#include "StatsHolderTmp.h"

namespace UserStructs {


    void StatsHolderResult::MergeFromTmp(UserStructs::StatsHolderTmp &tmp) {
        for (const auto &[host, unused]: tmp.HostnamesStats)
            if (HostnamesStats.find(host)
                == HostnamesStats.end()) {
                HostnamesStats.insert(*tmp.HostnamesStats.find(host));
                BOOST_LOG_TRIVIAL(debug) << "Hostname don't found in result statholder thus add new node in map";

            } else {
                HostnamesStats[host] += tmp.HostnamesStats[host];
                BOOST_LOG_TRIVIAL(debug) << "Hostname found, update stats in result statholder";

            }


        tmp.HostnamesStats.clear();
    }

    void StatsHolderResult::printStats() const {
        std::cout << std::endl
                  << "\tTraffic Analysis\t";
        for (const auto &[Key, Struct]: HostnamesStats) {
            std::cout << std::endl;
            if (Struct.oHostname.has_value()) {
                std::cout << "Hostname:  " << Struct.oHostname.value() << "\t";
            } else { std::cout << "Problems with hostname identifying\n Ipv4addr:  " << Struct.IpAddr; }
            std::cout << "Up/Down (bytes):  " << Struct.Up << "/" << Struct.Down << "\t"
                      << "Detected Packets Count (Up/Down):  " << Struct.UpPacketsCount << "/"
                      << Struct.DownPacketsCount << "\t"
                      << "Summarized traffic associated with host:  " << Struct.Up + Struct.Down;
        }
        std::cout << std::endl;
    }

}