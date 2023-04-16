add_executable(${PROJECT_NAME} main.cpp StatsHolderGlobal.cpp StatsHolderGlobal.h StatsHolderTmp.cpp StatsHolderTmp.h)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)