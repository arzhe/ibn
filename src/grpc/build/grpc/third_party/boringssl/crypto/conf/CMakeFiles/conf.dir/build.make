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
include grpc/third_party/boringssl/crypto/conf/CMakeFiles/conf.dir/depend.make

# Include the progress variables for this target.
include grpc/third_party/boringssl/crypto/conf/CMakeFiles/conf.dir/progress.make

# Include the compile flags for this target's objects.
include grpc/third_party/boringssl/crypto/conf/CMakeFiles/conf.dir/flags.make

grpc/third_party/boringssl/crypto/conf/CMakeFiles/conf.dir/conf.c.o: grpc/third_party/boringssl/crypto/conf/CMakeFiles/conf.dir/flags.make
grpc/third_party/boringssl/crypto/conf/CMakeFiles/conf.dir/conf.c.o: /home/gz/repository/workplace/ibn/third_part/grpc/third_party/boringssl/crypto/conf/conf.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gz/repository/workplace/ibn/src/grpc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object grpc/third_party/boringssl/crypto/conf/CMakeFiles/conf.dir/conf.c.o"
	cd /home/gz/repository/workplace/ibn/src/grpc/build/grpc/third_party/boringssl/crypto/conf && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/conf.dir/conf.c.o   -c /home/gz/repository/workplace/ibn/third_part/grpc/third_party/boringssl/crypto/conf/conf.c

grpc/third_party/boringssl/crypto/conf/CMakeFiles/conf.dir/conf.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/conf.dir/conf.c.i"
	cd /home/gz/repository/workplace/ibn/src/grpc/build/grpc/third_party/boringssl/crypto/conf && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/gz/repository/workplace/ibn/third_part/grpc/third_party/boringssl/crypto/conf/conf.c > CMakeFiles/conf.dir/conf.c.i

grpc/third_party/boringssl/crypto/conf/CMakeFiles/conf.dir/conf.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/conf.dir/conf.c.s"
	cd /home/gz/repository/workplace/ibn/src/grpc/build/grpc/third_party/boringssl/crypto/conf && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/gz/repository/workplace/ibn/third_part/grpc/third_party/boringssl/crypto/conf/conf.c -o CMakeFiles/conf.dir/conf.c.s

conf: grpc/third_party/boringssl/crypto/conf/CMakeFiles/conf.dir/conf.c.o
conf: grpc/third_party/boringssl/crypto/conf/CMakeFiles/conf.dir/build.make

.PHONY : conf

# Rule to build all files generated by this target.
grpc/third_party/boringssl/crypto/conf/CMakeFiles/conf.dir/build: conf

.PHONY : grpc/third_party/boringssl/crypto/conf/CMakeFiles/conf.dir/build

grpc/third_party/boringssl/crypto/conf/CMakeFiles/conf.dir/clean:
	cd /home/gz/repository/workplace/ibn/src/grpc/build/grpc/third_party/boringssl/crypto/conf && $(CMAKE_COMMAND) -P CMakeFiles/conf.dir/cmake_clean.cmake
.PHONY : grpc/third_party/boringssl/crypto/conf/CMakeFiles/conf.dir/clean

grpc/third_party/boringssl/crypto/conf/CMakeFiles/conf.dir/depend:
	cd /home/gz/repository/workplace/ibn/src/grpc/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gz/repository/workplace/ibn/src/grpc /home/gz/repository/workplace/ibn/third_part/grpc/third_party/boringssl/crypto/conf /home/gz/repository/workplace/ibn/src/grpc/build /home/gz/repository/workplace/ibn/src/grpc/build/grpc/third_party/boringssl/crypto/conf /home/gz/repository/workplace/ibn/src/grpc/build/grpc/third_party/boringssl/crypto/conf/CMakeFiles/conf.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : grpc/third_party/boringssl/crypto/conf/CMakeFiles/conf.dir/depend

