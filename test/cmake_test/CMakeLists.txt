cmake_minimum_required(VERSION 3.15)

project(GetFileNameComponent)

set(FullFileName "./test.txt.sh")

message(STATUS "original file name        : ${FullFileName}")

get_filename_component(directory ${FullFileName} DIRECTORY)
message(STATUS "Option: DIRECTORY         = ${directory}")

get_filename_component(name ${FullFileName} NAME)
message(STATUS "Option: NAME              = ${name}")

get_filename_component(ext ${FullFileName} EXT)
message(STATUS "Option: EXT               = ${ext}")

get_filename_component(name_we ${FullFileName} NAME_WE)
message(STATUS "Option: NAME_WE           = ${name_we}")

get_filename_component(last_ext ${FullFileName} LAST_EXT)
message(STATUS "Option: LAST_EXT          = ${last_ext}")

get_filename_component(name_wle ${FullFileName} NAME_WLE)
message(STATUS "Option: NAME_WLE          = ${name_wle}")

get_filename_component(absolute ${FullFileName} ABSOLUTE)
message(STATUS "CMAKE_CURRENT_SOURCE_DIR  = ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Option: ABSOLUTE          = ${absolute}")

get_filename_component(absolute_define_base ${FullFileName} ABSOLUTE BASE_DIR "/base_dir")
message(STATUS "Option: ABSOLUTE BASE_DIR = ${absolute_define_base}")
