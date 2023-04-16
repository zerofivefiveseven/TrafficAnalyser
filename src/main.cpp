#include <stdlib.h>
#include <queue>
#include <map>
#include "PcapLiveDeviceList.h"
#include "PcapFilter.h"
#include "TablePrinter.h"
#include "SystemUtils.h"
#include "PcapPlusPlusVersion.h"
#include <getopt.h>
#include <iostream>
#include "Packet.h"
#include "TcpLayer.h"
#include "HttpLayer.h"

//TODO move class implementation into .hpp/.cpp files
//TODO move headers includes/remove unused
//TODO use linter

#define EXIT_WITH_ERROR(reason) do { \
    printUsage(); \
    std::cout << std::endl << "ERROR: " << reason << std::endl << std::endl; \
    exit(1); \
    } while(0)


#define DEFAULT_CALC_RATES_PERIOD_SEC 5


static struct option HttpAnalyzerOptions[] =
        {
                {"interface",           optional_argument, nullptr, 'i'},
                {"rate-calc-period",    required_argument, nullptr, 'r'},
                {"disable-rates-print", no_argument,       nullptr, 'd'},
                {"list-interfaces",     no_argument,       nullptr, 'l'},
                {"help",                no_argument,       nullptr, 'h'},
                {nullptr, 0,                               nullptr, 0}
        };

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

    struct StatsHolderTmp : StatsHolderGlobal {
        friend StatsHolderGlobal;
    private:
        std::queue<std::pair<std::string, int>> requestQue;
    public:
        void UpdateByResponse(int ResponseDownSize) {
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

        void addRequestToQue(const std::pair<std::string, int> &tmp) {
            requestQue.push(tmp);
            std::cout << tmp.first << tmp.second;
        }
    };

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
}


/**
 * Print application usage
 */
void printUsage() {
    std::cout << std::endl
              << "Usage: Live traffic 80/443 port:" << std::endl
              << "-------------------------" << std::endl
              << pcpp::AppName::get() << " [-hld] [-r calc_period]" << std::endl
              << std::endl
              << "Options:" << std::endl
              << std::endl
              << "    -i interface   : Use the specified interface. Can be interface name (e.g eth0) or interface IPv4 address. For details use -l flag."
              << std::endl
              << "    -r calc_period : The period in seconds to calculate rates. If not provided default is 5 seconds"
              << std::endl
              << "    -d             : Disable periodic rates calculation" << std::endl
              << "    -h             : Displays this help message and exits" << std::endl
              << "    -l             : Print the list of interfaces and which is used and exists" << std::endl
              << std::endl;
}


/**
 * Go over all interfaces and output their names
 */
void listInterfaces() {
    const std::vector<pcpp::PcapLiveDevice *> &devList = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();

    std::cout
            << std::endl
            << "Network interfaces:"
            << std::endl
            << "This Name / IP used by default if -l flag not provided: "
            << devList.at(0)->getName()
            << " / "
            << devList.at(0)->getIPv4Address()
            << std::endl;
    for (auto iter: devList) {
        std::cout << "    -> Name: '" << iter->getName() << "'   IP address: " << iter->getIPv4Address().toString()
                  << std::endl;
    }
    exit(0);
}


/**
 * packet capture callback - called whenever a packet arrives
 */
void httpPacketArrive(pcpp::RawPacket *packet, pcpp::PcapLiveDevice *dev, void *cookie) {
    pcpp::Packet parsedPacket(packet);
    auto *statHolderTemp = (UserStructs::StatsHolderTmp *) cookie;
    if (parsedPacket.isPacketOfType(pcpp::HTTP)) {

        if (parsedPacket.isPacketOfType(pcpp::HTTPRequest)) {
            pcpp::HttpRequestLayer *httpRequestLayer = parsedPacket.getLayerOfType<pcpp::HttpRequestLayer>();
            if (httpRequestLayer == nullptr) {
                std::cerr << "Something went wrong, couldn't find HTTP request layer" << std::endl;

            }
            statHolderTemp->addRequestToQue(
                    std::pair<std::string, int>
                            (httpRequestLayer->getFieldByName(PCPP_HTTP_HOST_FIELD)->getFieldValue(),
                             httpRequestLayer->getLayerPayloadSize()));

            std::cout << std::endl
                      << "----------------------------------------------"
                      << std::endl
                      << "            REQUEST                           "
                      << std::endl
                      << "HTTP request URI: " << httpRequestLayer->getFirstLine()->getUri()
                      << std::endl
                      << "HTTP payload(in bytes): " << httpRequestLayer->getLayerPayloadSize()
                      << std::endl
                      << "HTTP request host: "
                      << httpRequestLayer->getFieldByName(PCPP_HTTP_HOST_FIELD)->getFieldValue()
                      << std::endl
                      << "FULL REQUEST TEXT: " << httpRequestLayer->toString()
                      << std::endl
                      << "HTTP data text: " << httpRequestLayer->getData()
                      << std::endl
                      << "------------------------------------------------"
                      << std::endl
                      << std::endl;


        } else if (parsedPacket.isPacketOfType(pcpp::HTTPResponse)) {
            auto *httpResponceLayer = parsedPacket.getLayerOfType<pcpp::HttpResponseLayer>();
            if (httpResponceLayer == nullptr) {
                std::cerr << "Something went wrong, couldn't find HTTP response layer" << std::endl;

            }


            std::cout << std::endl
                      << "----------------------------------------------"
                      << std::endl
                      << "            RESPONSE                         "
                      << std::endl
                      << "HTTP response content lengh(in bytes?): " << httpResponceLayer->getContentLength()
                      << std::endl
                      << "HTTP response text: " << httpResponceLayer->toString()
                      << std::endl
                      << "HTTP data text: " << httpResponceLayer->getData()
                      << std::endl
                      << "--------------------------------------------------"
                      << std::endl;
            statHolderTemp->UpdateByResponse(httpResponceLayer->getContentLength());
        }


    }


}


/**
 * The callback to be called when application is terminated by ctrl-c. Stops the endless while loop
 */
void onApplicationInterrupted(void *cookie) {
    bool *shouldStop = (bool *) cookie;
    *shouldStop = true;
}


/**
 * activate HTTP analysis from live traffic
 */
void analyzeHttpFromLiveTraffic(pcpp::PcapLiveDevice *dev, bool printRatesPeriodically, int printRatePeriod,
                                uint16_t dstPorthttp, uint16_t dstPorthttps) {
    // open the device
    if (!dev->open())
        EXIT_WITH_ERROR("Could not open the device");

    pcpp::OrFilter allFilters;
    pcpp::PortFilter httpPortFilterFirst(dstPorthttp, pcpp::SRC_OR_DST);
    pcpp::PortFilter httpPortFilterSecond(dstPorthttps, pcpp::SRC_OR_DST);
    allFilters.addFilter((pcpp::GeneralFilter * ) & httpPortFilterFirst);
    allFilters.addFilter((pcpp::GeneralFilter * ) & httpPortFilterSecond);
    if (!dev->setFilter(allFilters))
        EXIT_WITH_ERROR("Could not set up filter on device");


    UserStructs::StatsHolderTmp StatsInstanceTemp;

    UserStructs::StatsHolderGlobal StatsInstanceGlobal;
    // start capturing packets and collecting stats
    dev->startCapture(httpPacketArrive, &StatsInstanceTemp);


    // register the on app close event to print summary stats on app termination
    bool shouldStop = false;
    pcpp::ApplicationEventHandler::getInstance().onApplicationInterrupted(onApplicationInterrupted, &shouldStop);

    while (!shouldStop) {
        pcpp::multiPlatformSleep(printRatePeriod);

        // calculate rates
        if (printRatesPeriodically) {
            StatsInstanceTemp.printStats();
            StatsInstanceGlobal.MergeFromTmp(StatsInstanceTemp);

        }
    }

    // stop capturing and close the live device
    dev->stopCapture();
    dev->close();

    // calculate final rates
    StatsInstanceGlobal.MergeFromTmp(StatsInstanceTemp);
    // print stats summary
    StatsInstanceGlobal.printStats();

}

/**
 * main method of this utility
 */
int main(int argc, char *argv[]) {
    pcpp::AppName::init(argc, argv);

    std::string interfaceNameOrIP = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList().at(0)->getName();
    uint16_t porthttp = 80;
    uint16_t porthttps = 443;
    bool printRatesPeriodically = true;
    int printRatePeriod = DEFAULT_CALC_RATES_PERIOD_SEC;


    int optionIndex = 0;
    int opt = 0;

    while ((opt = getopt_long(argc, argv, "r:dhl", HttpAnalyzerOptions, &optionIndex)) != -1) {
        switch (opt) {
            case 0:
                break;
            case 'i':
                interfaceNameOrIP = optarg;
                break;
            case 'r':
                printRatePeriod = atoi(optarg);
                break;
            case 'd':
                printRatesPeriodically = false;
                break;
            case 'h':
                printUsage();
                exit(0);
            case 'l':
                listInterfaces();
                break;
            default:
                printUsage();
                exit(-1);
        }
    }
    if (interfaceNameOrIP == "") {
        EXIT_WITH_ERROR("No interface were provided");
    }
    pcpp::PcapLiveDevice *dev = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByIpOrName(
            interfaceNameOrIP);
    if (dev == nullptr)
        EXIT_WITH_ERROR("Couldn't find interface by provided IP address or name");

// start capturing and analyzing traffic
    analyzeHttpFromLiveTraffic(dev, printRatesPeriodically, printRatePeriod, porthttp, porthttps);
}
