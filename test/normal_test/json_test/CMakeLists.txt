cmake_minimum_required(VERSION 3.15)

project(json_test)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(RapidJSON)

add_executable(${PROJECT_NAME} json_test.cpp)
