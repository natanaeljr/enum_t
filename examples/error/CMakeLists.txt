cmake_minimum_required(VERSION 2.8)
project(simple)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-g -Werror -Wall -Wno-unused-variable")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_CXX_COMPILER STREQUAL g++)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-but-set-variable")
endif()

set(MXL_ENUM_INCLUDE_DIR ../../include)
include_directories(${MXL_ENUM_INCLUDE_DIR})
add_executable(error error.cc)
