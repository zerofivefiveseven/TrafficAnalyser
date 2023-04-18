cmake_minimum_required(VERSION 3.22.1)
project(testProj)
add_executable(${PROJECT_NAME} test_all.cpp)

## ProjectName as a target is not necessary but simple for me as newbie


target_include_directories(${PROJECT_NAME} PUBLIC ${PROJ_INCLUDE_DIRS} Exec_lib)
target_link_libraries(${PROJECT_NAME}  PUBLIC ${PROJ_LINK_LIBRARIES} GTest::gtest_main Exec_lib)
add_test(all_gtest ${PROJECT_NAME} )
