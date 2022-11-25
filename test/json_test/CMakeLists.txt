cmake_minimum_required(VERSION 3.15)

project(test)

#find_package(RapidJSON)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/../../src)
set(SRC_FILE
    ${SRC_DIR}/config/config.cpp
    ${SRC_DIR}/mgr/diag_mgr.cpp
    ${SRC_DIR}/log/logging.cpp
    ${SRC_DIR}/log/logstream.cpp
    ${SRC_DIR}/log/timestamp.cpp)

include_directories(${SRC_DIR})

add_executable(${PROJECT_NAME} test.cpp ${SRC_FILE})


