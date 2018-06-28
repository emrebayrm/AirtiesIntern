cmake_minimum_required(VERSION 3.10)
project(AirTies C)

set(CMAKE_C_STANDARD 11)

add_executable(AirTies matrix_lib.c matrix_lib.h main.c)