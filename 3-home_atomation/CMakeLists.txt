cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
project(home-atomation LANGUAGES CXX)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "source/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_subdirectory(tests)
