project(Exec)

add_library(${PROJECT_NAME}_lib StatsHolderResult.cpp StatsHolderResult.h StatsHolderTmp.cpp StatsHolderTmp.h)

add_executable(${PROJECT_NAME} main.cpp)

SET(Boost_USE_STATIC_LIBS ON)

target_include_directories(${PROJECT_NAME}_lib PUBLIC ${PROJ_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}_lib PUBLIC ${PROJ_LINK_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJ_INCLUDE_DIRS} ${PROJECT_NAME}_lib)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJ_LINK_LIBRARIES} ${PROJECT_NAME}_lib)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)