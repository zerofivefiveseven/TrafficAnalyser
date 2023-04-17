add_executable(${PROJECT_NAME} main.cpp StatsHolderResult.cpp StatsHolderResult.h StatsHolderTmp.cpp StatsHolderTmp.h)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)