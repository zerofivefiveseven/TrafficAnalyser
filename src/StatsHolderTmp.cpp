//
// Created by paul on 16.04.23.
//

#include "StatsHolderTmp.h"
#include "StatsHolderGlobal.h"

namespace UserStructs {
    void StatsHolderTmp::Update(const HostNameAssociated &tmp) {
        auto [IpAdrr, HttpHostname, UpSize, DownSize, Upcount, DownCount] = tmp;
        if (HostnamesStats.find(IpAdrr)
            == HostnamesStats.end()) {

            HostnamesStats.emplace(IpAdrr, HostNameAssociated{tmp});
//                statHolderTemp->HostnamesStats[tmpRequest.getFieldByName(PCPP_HTTP_HOST_FIELD)->getFieldValue()] =
//                        std::make_pair<long, long>(tmpRequest.getLayerPayloadSize(),httpResponceLayer->getContentLength());
        } else {
            HostnamesStats[IpAdrr] += HostNameAssociated{tmp};
        }
    }
}