cmake_minimum_required(VERSION 3.20)
project(Eindopdracht C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(.)

add_executable(Programmeren_1
        resources/lena.txt
        resources/lena512.bmp
        resources/Lena_klein.txt
        src/functions.h
        src/main.c)
