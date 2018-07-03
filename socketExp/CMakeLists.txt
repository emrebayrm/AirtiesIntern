cmake_minimum_required(VERSION 3.10)
project(socketExp)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(socketExp
        Makefile
        myether.c
        myether.h
        rx_raw.c
        tx_raw.c fileCapture.c concurrentFileCaptuure.c
        fileCap1.1/fileCapture.c)
