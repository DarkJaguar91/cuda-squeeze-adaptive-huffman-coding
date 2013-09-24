CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)
set(CMAKE_BUILD_TYPE Release)
set( CMAKE_CXX_FLAGS "-O3 -Wall -m64 -O3" )
add_library(timerLib Timer.cpp)