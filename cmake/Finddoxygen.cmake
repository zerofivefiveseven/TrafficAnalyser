include(FetchContent)
message("Fetching https://github.com/doxygen/doxygen.git")
FetchContent_Declare(doxygen
        GIT_REPOSITORY https://github.com/doxygen/doxygen.git
        GIT_TAG master
        GIT_PROGRESS on
        )
FetchContent_MakeAvailable(doxygen)