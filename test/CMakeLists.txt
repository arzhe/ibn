set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test_bin)

file(GLOB_RECURSE ibn_base_srcs "${CMAKE_SOURCE_DIR}/src/base/*.cpp")
file(GLOB_RECURSE ibn_log_srcs "${CMAKE_SOURCE_DIR}/src/log/*.cpp")

include_directories("${CMAKE_SOURCE_DIR}/src/")

add_executable(logfile_test logfile_test.cpp
                            ${ibn_log_srcs}
                            ${ibn_base_srcs})

add_test(NAME logfile_test COMMAND logfile_test)


