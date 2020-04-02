cmake_minimum_required(VERSION 2.8.9)

project(omega)

include_directories(include)

add_compile_options(-std=c++17)

file(GLOB SOURCES "src/*.cpp")

add_executable(omega ${SOURCES})