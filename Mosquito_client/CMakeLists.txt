cmake_minimum_required(VERSION 3.10)
project(Mosquito_client C)

set(CMAKE_C_STANDARD 11)

link_libraries(
        paho-mqtt3c
)

add_executable(Mosquito_client publisher.c subscriber.c)