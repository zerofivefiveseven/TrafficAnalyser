set(BOOST_ENABLE_CMAKE ON)
set(BOOST_INCLUDE_LIBRARIES thread filesystem system log)
include(FetchContent)
FetchContent_Declare(
        Boost
        GIT_REPOSITORY https://github.com/boostorg/boost.git
        GIT_TAG boost-1.80.0
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(Boost)