cmake_minimum_required(VERSION 3.26)
project(e_book_system C)

set(CMAKE_C_STANDARD 11)

add_executable(e_book_system
        main.c)
MATH(EXPR stack_size "100*1024*1024")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,${stack_size}")