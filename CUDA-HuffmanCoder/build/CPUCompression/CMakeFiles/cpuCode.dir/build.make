# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /home/bjtal/cuda-workspace/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bjtal/cuda-workspace/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/build

# Include any dependencies generated for this target.
include CPUCompression/CMakeFiles/cpuCode.dir/depend.make

# Include the progress variables for this target.
include CPUCompression/CMakeFiles/cpuCode.dir/progress.make

# Include the compile flags for this target's objects.
include CPUCompression/CMakeFiles/cpuCode.dir/flags.make

CPUCompression/CMakeFiles/cpuCode.dir/CPUmain.cpp.o: CPUCompression/CMakeFiles/cpuCode.dir/flags.make
CPUCompression/CMakeFiles/cpuCode.dir/CPUmain.cpp.o: ../CPUCompression/CPUmain.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/bjtal/cuda-workspace/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CPUCompression/CMakeFiles/cpuCode.dir/CPUmain.cpp.o"
	cd /home/bjtal/cuda-workspace/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/build/CPUCompression && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/cpuCode.dir/CPUmain.cpp.o -c /home/bjtal/cuda-workspace/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/CPUCompression/CPUmain.cpp

CPUCompression/CMakeFiles/cpuCode.dir/CPUmain.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cpuCode.dir/CPUmain.cpp.i"
	cd /home/bjtal/cuda-workspace/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/build/CPUCompression && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/bjtal/cuda-workspace/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/CPUCompression/CPUmain.cpp > CMakeFiles/cpuCode.dir/CPUmain.cpp.i

CPUCompression/CMakeFiles/cpuCode.dir/CPUmain.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cpuCode.dir/CPUmain.cpp.s"
	cd /home/bjtal/cuda-workspace/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/build/CPUCompression && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/bjtal/cuda-workspace/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/CPUCompression/CPUmain.cpp -o CMakeFiles/cpuCode.dir/CPUmain.cpp.s

CPUCompression/CMakeFiles/cpuCode.dir/CPUmain.cpp.o.requires:
.PHONY : CPUCompression/CMakeFiles/cpuCode.dir/CPUmain.cpp.o.requires

CPUCompression/CMakeFiles/cpuCode.dir/CPUmain.cpp.o.provides: CPUCompression/CMakeFiles/cpuCode.dir/CPUmain.cpp.o.requires
	$(MAKE) -f CPUCompression/CMakeFiles/cpuCode.dir/build.make CPUCompression/CMakeFiles/cpuCode.dir/CPUmain.cpp.o.provides.build
.PHONY : CPUCompression/CMakeFiles/cpuCode.dir/CPUmain.cpp.o.provides

CPUCompression/CMakeFiles/cpuCode.dir/CPUmain.cpp.o.provides.build: CPUCompression/CMakeFiles/cpuCode.dir/CPUmain.cpp.o

# Object files for target cpuCode
cpuCode_OBJECTS = \
"CMakeFiles/cpuCode.dir/CPUmain.cpp.o"

# External object files for target cpuCode
cpuCode_EXTERNAL_OBJECTS =

CPUCompression/libcpuCode.a: CPUCompression/CMakeFiles/cpuCode.dir/CPUmain.cpp.o
CPUCompression/libcpuCode.a: CPUCompression/CMakeFiles/cpuCode.dir/build.make
CPUCompression/libcpuCode.a: CPUCompression/CMakeFiles/cpuCode.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libcpuCode.a"
	cd /home/bjtal/cuda-workspace/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/build/CPUCompression && $(CMAKE_COMMAND) -P CMakeFiles/cpuCode.dir/cmake_clean_target.cmake
	cd /home/bjtal/cuda-workspace/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/build/CPUCompression && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cpuCode.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CPUCompression/CMakeFiles/cpuCode.dir/build: CPUCompression/libcpuCode.a
.PHONY : CPUCompression/CMakeFiles/cpuCode.dir/build

CPUCompression/CMakeFiles/cpuCode.dir/requires: CPUCompression/CMakeFiles/cpuCode.dir/CPUmain.cpp.o.requires
.PHONY : CPUCompression/CMakeFiles/cpuCode.dir/requires

CPUCompression/CMakeFiles/cpuCode.dir/clean:
	cd /home/bjtal/cuda-workspace/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/build/CPUCompression && $(CMAKE_COMMAND) -P CMakeFiles/cpuCode.dir/cmake_clean.cmake
.PHONY : CPUCompression/CMakeFiles/cpuCode.dir/clean

CPUCompression/CMakeFiles/cpuCode.dir/depend:
	cd /home/bjtal/cuda-workspace/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bjtal/cuda-workspace/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder /home/bjtal/cuda-workspace/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/CPUCompression /home/bjtal/cuda-workspace/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/build /home/bjtal/cuda-workspace/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/build/CPUCompression /home/bjtal/cuda-workspace/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/build/CPUCompression/CMakeFiles/cpuCode.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CPUCompression/CMakeFiles/cpuCode.dir/depend

