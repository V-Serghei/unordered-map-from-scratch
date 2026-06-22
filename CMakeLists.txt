cmake_minimum_required(VERSION 3.26)
project(unordered_map_final)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(unordered_map_final src/main.cpp)

target_include_directories(unordered_map_final PRIVATE include)

enable_testing()

add_test(NAME unordered_map_assertions COMMAND unordered_map_final)
