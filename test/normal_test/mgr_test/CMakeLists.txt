cmake_minimum_required(VERSION 3.15)

project(mgr_test)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/../../../src)
set(SRC_FILES 
    ${SRC_DIR}/config/config.cpp
    ${SRC_DIR}/mgr/domain_mgr.cpp
    ${SRC_DIR}/mgr/node_mgr.cpp
    ${SRC_DIR}/mgr/policy_mgr.cpp
    ${SRC_DIR}/log/logging.cpp
    ${SRC_DIR}/log/logstream.cpp
    ${SRC_DIR}/log/timestamp.cpp
    ${SRC_DIR}/base/util/util.cpp)

find_package(RapidJSON)

include_directories(${SRC_DIR})

add_executable(${PROJECT_NAME} mgr_test.cpp ${SRC_FILES})
