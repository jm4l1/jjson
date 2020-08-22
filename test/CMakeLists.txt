cmake_minimum_required(VERSION 3.7)

# Set the C++ standard we will use
set(CMAKE_CXX_STANDARD 17)

set(this jjsontests)

project(${this} C CXX)
set(sources src/jjsontests.cpp)

add_executable(${this} ${sources} "./catch.hpp")

set_target_properties(${this} PROPERTIES FOLDER tests)

target_include_directories(${this} PRIVATE ..)

target_link_directories(${this} PUBLIC jjson)

add_test(
    NAME ${this}
    COMMAND ${this}
)