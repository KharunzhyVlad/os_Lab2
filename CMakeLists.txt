cmake_minimum_required(VERSION 3.25)
project(OS_Lab2)

set(CMAKE_CXX_STANDARD 20)

add_library(test_lib SHARED test_lib.cpp test_lib.h)
add_executable(Lab2 main.cpp
        test_lib.h
        test_lib.cpp)
target_link_libraries(Lab2 PRIVATE test_lib)