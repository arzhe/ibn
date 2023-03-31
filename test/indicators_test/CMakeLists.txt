cmake_minimum_required(VERSION 2.8)

project(demo C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fpermissive -pthread")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/)

add_executable(${PROJECT_NAME} test.cpp)
