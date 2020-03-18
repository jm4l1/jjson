cmake_minimum_required(VERSION 3.0)

set(this jjsontests)

project(${this} C CXX)
set(sources src/jjsontests.cpp)
set(headers include/catch.hpp)

add_executable(${this} ${sources} ${headers})

set_target_properties(${this} PROPERTIES FOLDER tests)

target_include_directories(${this} PRIVATE ..)

target_link_directories(${this} PUBLIC jjson)

add_test(
    NAME ${this}
    COMMAND ${this}
)