cmake_minimum_required(VERSION 3.15)

project(test)

add_executable(${PROJECT_NAME} test.cpp main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR})
