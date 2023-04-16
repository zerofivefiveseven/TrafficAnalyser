
#include "StatsHolderGlobal.h"
#include "StatsHolderTmp.h"
#include "SystemUtils.h"
#include <getopt.h>
#include "PcapLiveDeviceList.h"
#include "PcapFilter.h"
#include "SystemUtils.h"
#include "PcapPlusPlusVersion.h"
#include "Packet.h"
#include "HttpLayer.h"
#include "IPv4Layer.h"

// TODO use log things BOOST::LOG
// TODO use doxygen
// TODO use Google Test
// TODO Use Docker
// TODO Specify pipeline with test, doxygen targets
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
            auto *httpRequestLayer = parsedPacket.getLayerOfType<pcpp::HttpRequestLayer>();
            pcpp::IPv4Layer *IPlayer = dynamic_cast<pcpp::IPv4Layer *>(parsedPacket.getLayerOfType(pcpp::IPv4));
            if (httpRequestLayer == nullptr) {
                std::cerr << "Something went wrong, couldn't find HTTP request layer" << std::endl;
            }
            statHolderTemp->Update(UserStructs::StatsHolderTmp::HostNameAssociated{IPlayer->getDstIPv4Address().toString(),
                                                                                   httpRequestLayer->getFieldByName(
                                                                                           PCPP_HTTP_HOST_FIELD)->getFieldValue(),
                                                                                   static_cast<unsigned >(httpRequestLayer->getLayerPayloadSize()),
                                                                                   0,
                                                                                   1,
                                                                                   0});

        } else if (parsedPacket.isPacketOfType(pcpp::HTTPResponse)) {
            auto *httpResponceLayer = parsedPacket.getLayerOfType<pcpp::HttpResponseLayer>();
            pcpp::IPv4Layer *IPlayer = dynamic_cast<pcpp::IPv4Layer *>(parsedPacket.getLayerOfType(pcpp::IPv4));
            if (httpResponceLayer == nullptr) {
                std::cerr << "Something went wrong, couldn't find HTTP response layer" << std::endl;

            }

            statHolderTemp->Update(UserStructs::StatsHolderTmp::HostNameAssociated{IPlayer->getSrcIPv4Address().toString(),
                                                                                   {},
                                                                                   0,
                                                                                   static_cast<unsigned >(httpResponceLayer->getContentLength()),
                                                                                   0,
                                                                                   1});
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
    allFilters.addFilter((pcpp::GeneralFilter *) &httpPortFilterFirst);
    allFilters.addFilter((pcpp::GeneralFilter *) &httpPortFilterSecond);
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
