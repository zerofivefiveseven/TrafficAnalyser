//
// Created by paul on 16.04.23.
//

#include "StatsHolderTmp.h"
#include "StatsHolderGlobal.h"

namespace UserStructs {
    void StatsHolderTmp::UpdateByResponse(unsigned ResponseDownSize) {
        if (requestQue.empty()) {
            return;
        }
        auto [HostName, RequestUpSize] = requestQue.front();
        if (HostnamesStats.find(HostName)
            == HostnamesStats.end()) {

            HostnamesStats.emplace(HostName, HostNameAssociated{HostName, RequestUpSize, ResponseDownSize, 1, 1});
//                statHolderTemp->HostnamesStats[tmpRequest.getFieldByName(PCPP_HTTP_HOST_FIELD)->getFieldValue()] =
//                        std::make_pair<long, long>(tmpRequest.getLayerPayloadSize(),httpResponceLayer->getContentLength());
        } else {
            HostnamesStats[HostName] += HostNameAssociated{HostName, RequestUpSize, ResponseDownSize, 1, 1};
        }
        requestQue.pop();
    }

    void StatsHolderTmp::addRequestToQue(const std::pair<std::string, unsigned> &tmp) {
        requestQue.push(tmp);
    }
}