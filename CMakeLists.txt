cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
project(tdd-tutorial LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_subdirectory(1-sprintf)
add_subdirectory(2-led)
add_subdirectory(3-home_automation)
