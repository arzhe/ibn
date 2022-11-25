cmake_minimum_required(VERSION 3.15)

project(unit_test)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/../../src)
set(SRC_FILES 
    ${SRC_DIR}/config/config.cpp)
    #${SRC_DIR}/mgr/domain_mgr.cpp
    #${SRC_DIR}/mgr/node_mgr.cpp
    #${SRC_DIR}/mgr/policy_mgr.cpp
    #${SRC_DIR}/log/logging.cpp
    #${SRC_DIR}/log/logstream.cpp
    #${SRC_DIR}/log/timestamp.cpp
    #${SRC_DIR}/base/util/util.cpp)

find_package(RapidJSON)
find_package(Threads REQUIRED)

include_directories(${SRC_DIR})

enable_testing()

add_executable(${PROJECT_NAME} config_test.cpp ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} gtest gtest_main pthread)

include(GoogleTest)
