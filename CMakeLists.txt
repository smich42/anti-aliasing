cmake_minimum_required(VERSION 3.15)
project(anti_aliasing C)

set(CMAKE_C_STANDARD 11)

add_executable(anti_aliasing main.c)

target_link_libraries(anti_aliasing SDL2)