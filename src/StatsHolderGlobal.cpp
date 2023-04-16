//
// Created by paul on 16.04.23.
//

#include "StatsHolderGlobal.h"
#include "StatsHolderTmp.h"

namespace UserStructs {


    void StatsHolderGlobal::MergeFromTmp(UserStructs::StatsHolderTmp &tmp) {
        for (const auto &[host, pair]: tmp.HostnamesStats) {
            if (HostnamesStats.find(host)
                == HostnamesStats.end()) {
                HostnamesStats.insert(*tmp.HostnamesStats.find(host));
//                statHolderTemp->HostnamesStats[tmpRequest.getFieldByName(PCPP_HTTP_HOST_FIELD)->getFieldValue()] =
//                        std::make_pair<long, long>(tmpRequest.getLayerPayloadSize(),httpResponceLayer->getContentLength());
            } else {
                HostnamesStats[host] += tmp.HostnamesStats[host];

            }

        }
        tmp.HostnamesStats.clear();
    }

    void StatsHolderGlobal::printStats() const {
        std::cout << std::endl
                  << "\tTraffic Analysis\t";
        for (const auto &[Key, Struct]: HostnamesStats) {
            std::cout << std::endl
                      << "Hostname:  " << Struct.Hostname << "\t"
                      //From long to double shadow cast
                      << "Up/Down (bytes):  " << Struct.Up << "/" << Struct.Down <<"\t"
                      << "Detected Packets Count (Up/Down):  " << Struct.UpPacketsCount << "/"
                      << Struct.DownPacketsCount << "\t"
                      << "Summarized traffic associated with host:  " << Struct.Up + Struct.Down;
        }
        std::cout << std::endl;
    }

}