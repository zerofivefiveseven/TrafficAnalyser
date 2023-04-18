
#include "StatsHolderResult.h"
#include "StatsHolderTmp.h"
#include "SystemUtils.h"
#include <getopt.h>
#include "PcapLiveDeviceList.h"
#include "PcapFilter.h"
#include "Packet.h"
#include "HttpLayer.h"
#include "IPv4Layer.h"
// TODO use log things BOOST::LOG
// TODO use Google Test
// TODO Use Docker
// TODO Specify pipeline with test, doxygen targets by Compound

void init_logging() {
    boost::log::core::get()->set_filter(
            boost::log::trivial::severity >= boost::log::trivial::info
    );
}


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
    BOOST_LOG_TRIVIAL(trace) << "Packet Arrives and Callback Called";

    pcpp::Packet parsedPacket(packet);
    auto *statHolderTemp = (UserStructs::StatsHolderTmp *) cookie;

    if (parsedPacket.isPacketOfType(pcpp::HTTP)) {
        BOOST_LOG_TRIVIAL(trace) << "Packet recognized as HTTP";
        if (parsedPacket.isPacketOfType(pcpp::HTTPRequest)) {
            BOOST_LOG_TRIVIAL(trace) << "Packet recognized as HTTP Request";

            auto *httpRequestLayer = parsedPacket.getLayerOfType<pcpp::HttpRequestLayer>();
            pcpp::IPv4Layer *IPlayerInstance = dynamic_cast<pcpp::IPv4Layer *>(parsedPacket.getLayerOfType(pcpp::IPv4));

            BOOST_LOG_TRIVIAL(trace) << "Trying to add packet to Holder";
            statHolderTemp->Update(
                    UserStructs::StatsHolderTmp::HostNameAssociated{IPlayerInstance->getDstIPv4Address().toString(),
                                                                    httpRequestLayer->getFieldByName(
                                                                            PCPP_HTTP_HOST_FIELD)->getFieldValue(),
                                                                    static_cast<unsigned >(httpRequestLayer->getLayerPayloadSize()),
                                                                    0,
                                                                    1,
                                                                    0});


        } else if (parsedPacket.isPacketOfType(pcpp::HTTPResponse)) {
            BOOST_LOG_TRIVIAL(trace) << "Packet recognized as HTTP Responce";

            auto *httpResponceLayer = parsedPacket.getLayerOfType<pcpp::HttpResponseLayer>();
            pcpp::IPv4Layer *IPlayerInstance = dynamic_cast<pcpp::IPv4Layer *>(parsedPacket.getLayerOfType(pcpp::IPv4));

            BOOST_LOG_TRIVIAL(trace) << "Trying to add packet to Holder";

            statHolderTemp->Update(
                    UserStructs::StatsHolderTmp::HostNameAssociated{IPlayerInstance->getSrcIPv4Address().toString(),
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
    BOOST_LOG_TRIVIAL(debug) << "app interrupted by SIGINT";
    *shouldStop = true;
}


/**
 * activate HTTP analysis from live traffic
 */
void analyzeHttpFromLiveTraffic(pcpp::PcapLiveDevice *dev, bool printRatesPeriodically, int printRatePeriod,
                                uint16_t dstPorthttp, uint16_t dstPorthttps) {
    // open the device
    if (!dev->open())
        BOOST_LOG_TRIVIAL(fatal) << "Could not open the device";

    pcpp::OrFilter allFilters;
    pcpp::PortFilter httpPortFilterFirst(dstPorthttp, pcpp::SRC_OR_DST);
    pcpp::PortFilter httpPortFilterSecond(dstPorthttps, pcpp::SRC_OR_DST);
    allFilters.addFilter((pcpp::GeneralFilter *) &httpPortFilterFirst);
    allFilters.addFilter((pcpp::GeneralFilter *) &httpPortFilterSecond);
    if (!dev->setFilter(allFilters))
        BOOST_LOG_TRIVIAL(fatal) << "Could not open the device";


    UserStructs::StatsHolderTmp StatsInstanceTemp;

    UserStructs::StatsHolderResult StatsInstanceGlobal;
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
    init_logging();
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
                BOOST_LOG_TRIVIAL(debug) << "Interface added and now is: " <<interfaceNameOrIP;
                break;
            case 'r':
                printRatePeriod = atoi(optarg);
                BOOST_LOG_TRIVIAL(debug) << "Rate Period is " <<printRatePeriod;
                break;
            case 'd':
                printRatesPeriodically = false;
                BOOST_LOG_TRIVIAL(debug) << "Now don't print stats periodically";
                break;
            case 'h':
                BOOST_LOG_TRIVIAL(trace) << "Printing Usage";
                printUsage();
                exit(0);
            case 'l':
                BOOST_LOG_TRIVIAL(trace) << "Print Interfaces List";
                listInterfaces();
                break;
            default:
                printUsage();
                exit(-1);
        }
    }
    if (interfaceNameOrIP == "") {
        BOOST_LOG_TRIVIAL(fatal) << "No interface were provided";
    }
    pcpp::PcapLiveDevice *dev = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByIpOrName(
            interfaceNameOrIP);
    if (dev == nullptr)
        BOOST_LOG_TRIVIAL(fatal) << "Couldn't find interface by provided IP address or name";

// start capturing and analyzing traffic
    analyzeHttpFromLiveTraffic(dev, printRatesPeriodically, printRatePeriod, porthttp, porthttps);
}
