cmake_minimum_required(VERSION 3.2)
project(hash_map)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(hash_map ${SOURCE_FILES} hash_map.h hash_map.cpp)