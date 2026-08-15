cmake_minimum_required(VERSION 3.8)
project(first_project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(first first.cpp)
add_executable(main main.cpp)
target_link_libraries(main first)
