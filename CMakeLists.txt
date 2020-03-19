cmake_minimum_required(VERSION 3.15)
project(anti_aliasing C)

set(CMAKE_C_STANDARD 11)

add_executable(anti_aliasing main.c demo.h demo.c draw.h draw.c antialiasing.c antialiasing.h macros.h)

target_link_libraries(anti_aliasing SDL2)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/in.txt ${CMAKE_CURRENT_BINARY_DIR}/in.txt COPYONLY)