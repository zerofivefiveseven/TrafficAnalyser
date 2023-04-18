cmake_minimum_required(VERSION 3.22.1)
project(testProj)
add_executable(testExec test_all.cpp)


#add_custom_target(tests
#COMMAND test
#DEPENDS test
#WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#)

target_include_directories(testExec PUBLIC ${PROJ_INCLUDE_DIRS} Exec_lib)
target_link_libraries(testExec PUBLIC ${PROJ_LINK_LIBRARIES} GTest::gtest_main Exec_lib)
add_test(all_gtest testExec)
