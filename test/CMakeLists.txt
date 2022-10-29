cmake_minimum_required(VERSION 3.10)

project(test)

include_directories(../source)

add_executable(plus_test plus_test.cpp)
target_link_libraries(plus_test ZongHeng)
