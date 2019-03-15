cmake_minimum_required(VERSION 2.8)
project(simple)
set(CMAKE_CXX_STANDARD 17)

set(MXL_ENUM_INCLUDE_DIR ../../include)
include_directories(${MXL_ENUM_INCLUDE_DIR})
add_executable(error error.cc)
