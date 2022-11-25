cmake_minimum_required(VERSION 3.15)

project(log_test)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/../../src)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-fpermissive ${CMAKE_CXX_FLAGS}")

include_directories(${SRC_DIR})


set(SRC_FILE
     ${SRC_DIR}/base/util/util.cpp
     ${SRC_DIR}/log/logging.cpp
     ${SRC_DIR}/log/logstream.cpp
     ${SRC_DIR}/log/timestamp.cpp)

add_executable(${PROJECT_NAME} log_test.cpp ${SRC_FILE})
