cmake_minimum_required(VERSION 3.22.1)
## TODO Include logger, test platform
## TODO switch to Release Type
## TODO enable compile options
## TODO enable documentation
## TODO enable DOCKER
## TODO switch executable location
project(Analyser CXX)
enable_testing()
##set(${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})


#add_executable(${PROJECT_NAME} out/build/_deps/PcapPlusPlus/Examples/DNSResolver/main.cpp)
#Compiler Options env variables/etc.

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
#set(ENV{PKG_CONFIG_PATH}
#        ${CMAKE_SOURCE_DIR}/dependencies/lib/pkgconfig/:${CMAKE_SOURCE_DIR}/dependencies/lib64/pkgconfig/)
SET(Boost_USE_STATIC_LIBS ON)

find_package(PCAP)
find_package(PcapPlusPlus)
find_package(Threads REQUIRED)
find_package(googletest)
find_package(Boost 1.80.0 REQUIRED COMPONENTS log log-setup)

#find_package(doxygen)



set(PROJ_INCLUDE_DIRS
        ${Boost_INCLUDE_DIRS}
        ${PCAP_INCLUDE_DIR}
        out/build/_deps/pcapplusplus-src/Common++/header
        out/build/_deps/pcapplusplus-src/Pcap++/header
        out/build/_deps/pcapplusplus-src/Packet++/header)

set(PROJ_LINK_LIBRARIES
        ${Boost_LIBRARIES}
        pthread
        Boost::log_setup
        Boost::log
        ${PCAP_LIBRARY}
        Threads::Threads
        Common++
        Pcap++
        Packet++
        ${Boost_LIBRARY_DIRS}
        )
#doxygen
#doxygen::doxygen

#include(test/CMakeLists.txt)
add_subdirectory(test)
add_subdirectory(src)