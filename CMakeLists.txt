cmake_minimum_required(VERSION 3.22.1)
project(Analyser)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(src)
#Compiler Options env variables/etc.



#list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/out/build/_deps/pcapplusplus-build")
#list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/out/build/_deps/pcapplusplus-build")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/out/build/_deps/")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
#set(ENV{PKG_CONFIG_PATH}
#        ${CMAKE_SOURCE_DIR}/dependencies/lib/pkgconfig/:${CMAKE_SOURCE_DIR}/dependencies/lib64/pkgconfig/)
find_package(PcapPlusPlus)
find_package(Threads REQUIRED)
find_package(PCAP)
find_package(googletest)


SET(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.80.0 REQUIRED COMPONENTS log log-setup)
#sudo apt-get install pkg-config

link_directories(${Boost_LIBRARY_DIRS})

set(PROJ_INCLUDE_DIRS
        ${Boost_INCLUDE_DIRS}
        ${PCAP_INCLUDE_DIR}
        ${PcapPlusPlus_INCLUDE_DIRS})

set(PROJ_LINK_LIBRARIES
        ${Boost_LIBRARIES}
        pthread
        Boost::log_setup
        Boost::log
        ${PCAP_LIBRARY}
        Threads::Threads
        ${PcapPlusPlus_LIBRARIES})


target_include_directories(${PROJECT_NAME} PUBLIC ${PROJ_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJ_LINK_LIBRARIES})