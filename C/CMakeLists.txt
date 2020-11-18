cmake_minimum_required(VERSION 3.17)
project(Ambilight)

set(CMAKE_C_STANDARD 11)

add_executable(ambilight main.c screen.c)

target_link_libraries(ambilight X11 config)

add_executable(ambilight-color background.c)
target_link_libraries(ambilight-color config)