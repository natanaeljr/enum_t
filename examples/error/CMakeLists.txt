cmake_minimum_required(VERSION 2.8)
project(simple)

set(CMAKE_CXX_STANDARD 14)

include_directories(../../include)
add_executable(error
    error.cc
)
