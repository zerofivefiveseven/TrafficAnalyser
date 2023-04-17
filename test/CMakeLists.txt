cmake_minimum_required(VERSION 3.23)

add_executable(test
src/main.cpp
src/StatsHolderResult.cpp
src/StatsHolderResult.h
src/StatsHolderTmp.cpp
src/StatsHolderTmp.h
        test/test_all.cpp)


add_custom_target(tests
COMMAND test
DEPENDS test
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test
)

target_include_directories(test PUBLIC ${PROJ_INCLUDE_DIRS})
target_link_libraries(test PUBLIC ${PROJ_LINK_LIBRARIES} GTest::gtest_main)
# Link Shlwapi to the project