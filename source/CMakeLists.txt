cmake_minimum_required(VERSION 3.10)
project(ZongHeng)

set(CMAKE_CXX_STANDARD 11)

set(sources Qin.cpp)

add_library(ZongHeng STATIC ${sources})
target_include_directories(ZongHeng PUBLIC .)
