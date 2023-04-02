# FindPcapPlusPlus.cmake
#
# Finds the PcapPlusPlus library.
#
# This will define the following variables
#
#    PcapPlusPlus_FOUND
#    PcapPlusPlus_INCLUDE_DIRS
#    PcapPlusPlus_LIBRARIES
#    PcapPlusPlus_VERSION
#
# and the following imported targets
#
#    PcapPlusPlus::PcapPlusPlus
#

#include( ExternalProject )
#
#set( boost_URL "http://sourceforge.net/projects/boost/files/boost/1.63.0/boost_1_63_0.tar.bz2" )
#set( boost_SHA1 "9f1dd4fa364a3e3156a77dc17aa562ef06404ff6" )
#set( boost_INSTALL ${CMAKE_CURRENT_BINARY_DIR}/third_party/boost )
#set( boost_INCLUDE_DIR ${boost_INSTALL}/include )
#set( boost_LIB_DIR ${boost_INSTALL}/lib )
#
#ExternalProject_Add( boost
#        PREFIX boost
#        URL ${boost_URL}
#        URL_HASH SHA1=${boost_SHA1}
#        BUILD_IN_SOURCE 1
#        CONFIGURE_COMMAND
#        ./bootstrap.sh
#        --with-libraries=filesystem
#        --with-libraries=system
#        --with-libraries=date_time
#        --prefix=<INSTALL_DIR>
#        BUILD_COMMAND
#        ./b2 install link=static variant=release threading=multi runtime-link=static
#        INSTALL_COMMAND ""
#        INSTALL_DIR ${boost_INSTALL} )
#
#set( Boost_LIBRARIES
#        ${boost_LIB_DIR}/libboost_filesystem.a
#        ${boost_LIB_DIR}/libboost_system.a
#        ${boost_LIB_DIR}/libboost_date_time.a )
#message( STATUS "Boost static libs: " ${Boost_LIBRARIES} )

#include(FetchContent)
#
#message("Fetching https://github.com/seladb/PcapPlusPlus.git")
#FetchContent_Declare(
#        PcapPlusPlus
#        GIT_REPOSITORY https://github.com/seladb/PcapPlusPlus.git
#        GIT_TAG origin/master
#        GIT_PROGRESS TRUE
#        OVERRIDE_FIND_PACKAGE
#)
#
#FetchContent_MakeAvailable(PcapPlusPlus)


if (PC_PcapPlusPlus_INCLUDEDIR AND PC_PcapPlusPlus_LIBDIR)
    set(PcapPlusPlus_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig REQUIRED)
pkg_check_modules(PC_PcapPlusPlus REQUIRED IMPORTED_TARGET PcapPlusPlus)

set(PcapPlusPlus_VERSION ${PC_PcapPlusPlus_VERSION})

mark_as_advanced(PcapPlusPlus_INCLUDE_DIR PcapPlusPlus_LIBRARY)

foreach (LIB_NAME ${PC_PcapPlusPlus_LIBRARIES})
    find_library(${LIB_NAME}_PATH ${LIB_NAME} HINTS ${PC_PcapPlusPlus_LIBDIR})
    if (${LIB_NAME}_PATH)
        list(APPEND PcapPlusPlus_LIBS ${${LIB_NAME}_PATH})
    endif ()
endforeach ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PcapPlusPlus
        REQUIRED_VARS PC_PcapPlusPlus_INCLUDEDIR PC_PcapPlusPlus_LIBDIR
        VERSION_VAR PcapPlusPlus_VERSION
        )

if (PcapPlusPlus_FOUND)
    set(PcapPlusPlus_INCLUDE_DIRS ${PC_PcapPlusPlus_INCLUDEDIR})
    set(PcapPlusPlus_LIBRARIES ${PcapPlusPlus_LIBS})
endif ()

if (PcapPlusPlus_FOUND AND NOT TARGET PcapPlusPlus::PcapPlusPlus)
    add_library(PcapPlusPlus::PcapPlusPlus INTERFACE IMPORTED)
    set_target_properties(PcapPlusPlus::PcapPlusPlus PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${PcapPlusPlus_INCLUDE_DIRS}"
            INTERFACE_LINK_LIBRARIES "${PcapPlusPlus_LIBRARIES}"
            INTERFACE_COMPILE_FLAGS "${PC_PcapPlusPlus_CFLAGS}"
            )
endif ()