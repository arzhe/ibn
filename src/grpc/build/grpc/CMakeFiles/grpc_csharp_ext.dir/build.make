# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/gz/repository/workplace/ibn/src/grpc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/gz/repository/workplace/ibn/src/grpc/build

# Include any dependencies generated for this target.
include grpc/CMakeFiles/grpc_csharp_ext.dir/depend.make

# Include the progress variables for this target.
include grpc/CMakeFiles/grpc_csharp_ext.dir/progress.make

# Include the compile flags for this target's objects.
include grpc/CMakeFiles/grpc_csharp_ext.dir/flags.make

grpc/CMakeFiles/grpc_csharp_ext.dir/src/csharp/ext/grpc_csharp_ext.c.o: grpc/CMakeFiles/grpc_csharp_ext.dir/flags.make
grpc/CMakeFiles/grpc_csharp_ext.dir/src/csharp/ext/grpc_csharp_ext.c.o: /home/gz/repository/workplace/ibn/third_part/grpc/src/csharp/ext/grpc_csharp_ext.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gz/repository/workplace/ibn/src/grpc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object grpc/CMakeFiles/grpc_csharp_ext.dir/src/csharp/ext/grpc_csharp_ext.c.o"
	cd /home/gz/repository/workplace/ibn/src/grpc/build/grpc && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/grpc_csharp_ext.dir/src/csharp/ext/grpc_csharp_ext.c.o   -c /home/gz/repository/workplace/ibn/third_part/grpc/src/csharp/ext/grpc_csharp_ext.c

grpc/CMakeFiles/grpc_csharp_ext.dir/src/csharp/ext/grpc_csharp_ext.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/grpc_csharp_ext.dir/src/csharp/ext/grpc_csharp_ext.c.i"
	cd /home/gz/repository/workplace/ibn/src/grpc/build/grpc && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/gz/repository/workplace/ibn/third_part/grpc/src/csharp/ext/grpc_csharp_ext.c > CMakeFiles/grpc_csharp_ext.dir/src/csharp/ext/grpc_csharp_ext.c.i

grpc/CMakeFiles/grpc_csharp_ext.dir/src/csharp/ext/grpc_csharp_ext.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/grpc_csharp_ext.dir/src/csharp/ext/grpc_csharp_ext.c.s"
	cd /home/gz/repository/workplace/ibn/src/grpc/build/grpc && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/gz/repository/workplace/ibn/third_part/grpc/src/csharp/ext/grpc_csharp_ext.c -o CMakeFiles/grpc_csharp_ext.dir/src/csharp/ext/grpc_csharp_ext.c.s

# Object files for target grpc_csharp_ext
grpc_csharp_ext_OBJECTS = \
"CMakeFiles/grpc_csharp_ext.dir/src/csharp/ext/grpc_csharp_ext.c.o"

# External object files for target grpc_csharp_ext
grpc_csharp_ext_EXTERNAL_OBJECTS =

grpc/libgrpc_csharp_ext.so: grpc/CMakeFiles/grpc_csharp_ext.dir/src/csharp/ext/grpc_csharp_ext.c.o
grpc/libgrpc_csharp_ext.so: grpc/CMakeFiles/grpc_csharp_ext.dir/build.make
grpc/libgrpc_csharp_ext.so: grpc/libgrpc.a
grpc/libgrpc_csharp_ext.so: grpc/libgpr.a
grpc/libgrpc_csharp_ext.so: grpc/third_party/boringssl/ssl/libssl.a
grpc/libgrpc_csharp_ext.so: grpc/third_party/boringssl/crypto/libcrypto.a
grpc/libgrpc_csharp_ext.so: grpc/third_party/zlib/libz.a
grpc/libgrpc_csharp_ext.so: grpc/third_party/cares/cares/lib/libcares.a
grpc/libgrpc_csharp_ext.so: grpc/libaddress_sorting.a
grpc/libgrpc_csharp_ext.so: grpc/CMakeFiles/grpc_csharp_ext.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/gz/repository/workplace/ibn/src/grpc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libgrpc_csharp_ext.so"
	cd /home/gz/repository/workplace/ibn/src/grpc/build/grpc && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/grpc_csharp_ext.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
grpc/CMakeFiles/grpc_csharp_ext.dir/build: grpc/libgrpc_csharp_ext.so

.PHONY : grpc/CMakeFiles/grpc_csharp_ext.dir/build

grpc/CMakeFiles/grpc_csharp_ext.dir/clean:
	cd /home/gz/repository/workplace/ibn/src/grpc/build/grpc && $(CMAKE_COMMAND) -P CMakeFiles/grpc_csharp_ext.dir/cmake_clean.cmake
.PHONY : grpc/CMakeFiles/grpc_csharp_ext.dir/clean

grpc/CMakeFiles/grpc_csharp_ext.dir/depend:
	cd /home/gz/repository/workplace/ibn/src/grpc/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gz/repository/workplace/ibn/src/grpc /home/gz/repository/workplace/ibn/third_part/grpc /home/gz/repository/workplace/ibn/src/grpc/build /home/gz/repository/workplace/ibn/src/grpc/build/grpc /home/gz/repository/workplace/ibn/src/grpc/build/grpc/CMakeFiles/grpc_csharp_ext.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : grpc/CMakeFiles/grpc_csharp_ext.dir/depend

