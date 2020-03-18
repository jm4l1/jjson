cmake_minimum_required(VERSION 3.0)
set(project jjson)
project(${project} C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

enable_testing()

set(headers include/jjson.hpp)
set(sources src/jjson.cpp)

add_library(${project} STATIC ${sources} ${headers})

add_subdirectory(test)