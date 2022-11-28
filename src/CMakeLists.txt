# Copyright 2018 gRPC authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# cmake build file for C++ helloworld example.
# Assumes protobuf and gRPC have been installed using cmake.
# See cmake_externalproject/CMakeLists.txt for all-in-one cmake build
# that automatically builds all the dependencies before building helloworld.

# cmake_minimum_required(VERSION 2.8)

# project(Intend C CXX)

# option(IBN_LOG_TRACE "trace log" ON)
# if(IBN_LOG_TRACE)
#     add_definitions(-DIBN_LOG_TRACE)
# endif()

# if(NOT MSVC)
#   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# else()
#   add_definitions(-D_WIN32_WINNT=0x600)
# endif()

#if(0)
#  # One way to build a projects that uses gRPC is to just include the
#  # entire gRPC project tree via "add_subdirectory".
#  # This approach is very simple to use, but the are some potential
#  # disadvantages:
#  # * it includes gRPC's CMakeLists.txt directly into your build script
#  #   without and that can make gRPC's internal setting interfere with your
#  #   own build.
#  # * depending on what's installed on your system, the contents of submodules
#  #   in gRPC's third_party/* might need to be available (and there might be
#  #   additional prerequisites required to build them). Consider using
#  #   the gRPC_*_PROVIDER options to fine-tune the expected behavior.
#  #
#  # A more robust approach to add dependency on gRPC is using
#  # cmake's ExternalProject_Add (see cmake_externalproject/CMakeLists.txt).
  
#  # Include the gRPC's cmake build (normally grpc source code would live
#  # in a git submodule called "third_party/grpc", but this example lives in
#  # the same repository as gRPC sources, so we just look a few directories up)
#  add_subdirectory(${CMAKE_SOURCE_DIR}/../../third_part/grpc ${CMAKE_CURRENT_BINARY_DIR}/grpc EXCLUDE_FROM_ALL)
#  message(STATUS "Using gRPC via add_subdirectory.")
  
#  # After using add_subdirectory, we can now use the grpc targets directly from
#  # this build.
#  set(_PROTOBUF_LIBPROTOBUF libprotobuf)
#  set(_PROTOBUF_PROTOC $<TARGET_FILE:protoc>)
#  set(_GRPC_GRPCPP_UNSECURE grpc++_unsecure)
#  set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:grpc_cpp_plugin>)
#else()
#  # This branch assumes that gRPC and all its dependencies are already installed
#  # on this system, so they can be located by find_package().

#  # Find Protobuf installation
#  # Looks for protobuf-config.cmake file installed by Protobuf's cmake installation.
#  set(protobuf_MODULE_COMPATIBLE TRUE)
#  find_package(Protobuf CONFIG REQUIRED)
#  message(STATUS "Using protobuf ${protobuf_VERSION}")

#  set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
#  set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

#  # Find gRPC installation
#  # Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
#  find_package(gRPC CONFIG REQUIRED)
#  message(STATUS "Using gRPC ${gRPC_VERSION}")

#  set(_GRPC_GRPCPP_UNSECURE gRPC::grpc++_unsecure)
#  set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
#endif()

# Proto file
get_filename_component(hw_proto "${CMAKE_SOURCE_DIR}/example/doc/intend.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Generated sources
set(hw_proto_dir "${CMAKE_SOURCE_DIR}/src/proto")
set(hw_proto_srcs "${hw_proto_dir}/intend.pb.cc")
set(hw_proto_hdrs "${hw_proto_dir}/intend.pb.h")
set(hw_grpc_srcs "${hw_proto_dir}/intend.grpc.pb.cc")
set(hw_grpc_hdrs "${hw_proto_dir}/intend.grpc.pb.h")

set(ibn_grpc_src_dir "${CMAKE_SOURCE_DIR}/src/grpc")

file(GLOB_RECURSE ibn_base_srcs "${CMAKE_SOURCE_DIR}/src/base/*.cpp")
file(GLOB_RECURSE ibn_config_srcs "${CMAKE_SOURCE_DIR}/src/config/*.cpp")
file(GLOB_RECURSE ibn_log_srcs "${CMAKE_SOURCE_DIR}/src/log/*.cpp")
file(GLOB_RECURSE ibn_mgr_srcs "${CMAKE_SOURCE_DIR}/src/mgr/*.cpp")
file(GLOB_RECURSE ibn_parse_srcs "${CMAKE_SOURCE_DIR}/src/parse/*.cpp")

add_custom_command(
      OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${hw_proto_dir}"
        --cpp_out "${hw_proto_dir}"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
      DEPENDS "${hw_proto}")

# Include generated *.pb.h files
include_directories("${hw_proto_dir}/../")

# Targets greeter_[async_](client|server)
foreach(_target
  intend_client intend_server)
  add_executable(${_target} "${ibn_grpc_src_dir}/${_target}.cpp"
    ${hw_proto_srcs}
    ${hw_grpc_srcs}
    ${ibn_base_srcs}
    ${ibn_config_srcs}
    ${ibn_log_srcs}
    ${ibn_mgr_srcs}
    ${ibn_parse_srcs})
  target_link_libraries(${_target}
    ${_GRPC_GRPCPP_UNSECURE}
    ${_PROTOBUF_LIBPROTOBUF})
endforeach()

