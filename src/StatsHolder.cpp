//
// Created by paul on 16.04.23.
//

#include "StatsHolder.h"

namespace UserStructs {

    void StatsHolderTmp::UpdateByResponse(int ResponseDownSize) {
        if (requestQue.empty()) {
            return;
        }
        auto tmpRequest = requestQue.front();
        std::string HostName = tmpRequest.first;
        int RequestUpSize = tmpRequest.second;
        if (HostnamesStats.find(HostName)
            == HostnamesStats.end()) {

            HostnamesStats.emplace(HostName, std::make_pair<long, long>(RequestUpSize, ResponseDownSize));
//                statHolderTemp->HostnamesStats[tmpRequest.getFieldByName(PCPP_HTTP_HOST_FIELD)->getFieldValue()] =
//                        std::make_pair<long, long>(tmpRequest.getLayerPayloadSize(),httpResponceLayer->getContentLength());
        } else {
            HostnamesStats[HostName] =
                    std::make_pair<long, long>(HostnamesStats[HostName].first + RequestUpSize,
                                               HostnamesStats[HostName].second + ResponseDownSize);
        }
        requestQue.pop();
    }

    void StatsHolderTmp::addRequestToQue(const std::pair<std::string, int> &tmp) {
        requestQue.push(tmp);
        std::cout << tmp.first << tmp.second;
    }


    void StatsHolderGlobal::MergeFromTmp(UserStructs::StatsHolderTmp &tmp) {
        for (const auto &[host, pair]: tmp.HostnamesStats) {
            if (HostnamesStats.find(host)
                == HostnamesStats.end()) {
                HostnamesStats.insert(*tmp.HostnamesStats.find(host));
//                statHolderTemp->HostnamesStats[tmpRequest.getFieldByName(PCPP_HTTP_HOST_FIELD)->getFieldValue()] =
//                        std::make_pair<long, long>(tmpRequest.getLayerPayloadSize(),httpResponceLayer->getContentLength());
            } else {
                HostnamesStats[host] =
                        std::make_pair<long, long>(HostnamesStats[host].first + tmp.HostnamesStats[host].first,
                                                   HostnamesStats[host].second + tmp.HostnamesStats[host].second);

            }

        }
        tmp.HostnamesStats.clear();
    }

    void StatsHolderGlobal::printStats() const {
        std::cout << std::endl
                  << "\tTraffic Analysis\t";
        for (const auto &[k, v]: HostnamesStats) {
            std::cout << std::endl
                      << "Hostname:\t" << k << "\t"
                      //From long to double shadow cast
                      << "Up/Down (bytes)\t:" << v.first << "/" << v.second;
        }
        std::cout << std::endl;
    }
}