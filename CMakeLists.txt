cmake_minimum_required(VERSION 3.9)
project(latin1_to_utf8 C)

set(CMAKE_C_STANDARD 11)

add_executable(latin1_to_utf8 main.c)

if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif ()
if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif ()
