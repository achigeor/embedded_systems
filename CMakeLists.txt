cmake_minimum_required(VERSION 3.6)
project(embeded)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES time_measurements.c)
add_executable(embeded ${SOURCE_FILES})