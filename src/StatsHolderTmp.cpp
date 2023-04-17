//
// Created by paul on 16.04.23.
//

#include "StatsHolderTmp.h"
#include "StatsHolderResult.h"

namespace UserStructs {
    void StatsHolderTmp::Update(const HostNameAssociated &tmp) {
        auto [IpAdrr, HttpHostname, UpSize, DownSize, Upcount, DownCount] = tmp;
        if (HostnamesStats.find(IpAdrr)
            == HostnamesStats.end()) {

            HostnamesStats.emplace(IpAdrr, HostNameAssociated{tmp});
            BOOST_LOG_TRIVIAL(debug) << "Hostname don't found in temporary statholder thus add new node in map";

        } else {
            HostnamesStats[IpAdrr] += HostNameAssociated{tmp};
            BOOST_LOG_TRIVIAL(debug) << "Hostname found, update stats in temporary statholder";

        }
    }
}