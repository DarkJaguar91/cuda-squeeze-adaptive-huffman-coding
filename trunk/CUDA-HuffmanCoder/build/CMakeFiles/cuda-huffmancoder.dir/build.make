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
CMAKE_SOURCE_DIR = /home/bhugo/brandon/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bhugo/brandon/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/build

# Include any dependencies generated for this target.
include CMakeFiles/cuda-huffmancoder.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/cuda-huffmancoder.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cuda-huffmancoder.dir/flags.make

CMakeFiles/cuda-huffmancoder.dir/main.cpp.o: CMakeFiles/cuda-huffmancoder.dir/flags.make
CMakeFiles/cuda-huffmancoder.dir/main.cpp.o: ../main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/bhugo/brandon/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/cuda-huffmancoder.dir/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/cuda-huffmancoder.dir/main.cpp.o -c /home/bhugo/brandon/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/main.cpp

CMakeFiles/cuda-huffmancoder.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cuda-huffmancoder.dir/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/bhugo/brandon/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/main.cpp > CMakeFiles/cuda-huffmancoder.dir/main.cpp.i

CMakeFiles/cuda-huffmancoder.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cuda-huffmancoder.dir/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/bhugo/brandon/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/main.cpp -o CMakeFiles/cuda-huffmancoder.dir/main.cpp.s

CMakeFiles/cuda-huffmancoder.dir/main.cpp.o.requires:
.PHONY : CMakeFiles/cuda-huffmancoder.dir/main.cpp.o.requires

CMakeFiles/cuda-huffmancoder.dir/main.cpp.o.provides: CMakeFiles/cuda-huffmancoder.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/cuda-huffmancoder.dir/build.make CMakeFiles/cuda-huffmancoder.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/cuda-huffmancoder.dir/main.cpp.o.provides

CMakeFiles/cuda-huffmancoder.dir/main.cpp.o.provides.build: CMakeFiles/cuda-huffmancoder.dir/main.cpp.o

# Object files for target cuda-huffmancoder
cuda__huffmancoder_OBJECTS = \
"CMakeFiles/cuda-huffmancoder.dir/main.cpp.o"

# External object files for target cuda-huffmancoder
cuda__huffmancoder_EXTERNAL_OBJECTS =

cuda-huffmancoder: CMakeFiles/cuda-huffmancoder.dir/main.cpp.o
cuda-huffmancoder: CMakeFiles/cuda-huffmancoder.dir/build.make
cuda-huffmancoder: /usr/local/cuda-5.5/lib64/libcudart.so
cuda-huffmancoder: CPUCompression/libcpuCode.a
cuda-huffmancoder: GPUCompressor/libGPUCoder.a
cuda-huffmancoder: GPUCompression/libgpuCode.a
cuda-huffmancoder: Timer/libtimerLib.a
cuda-huffmancoder: /usr/local/cuda-5.5/lib64/libcudart.so
cuda-huffmancoder: CMakeFiles/cuda-huffmancoder.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable cuda-huffmancoder"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cuda-huffmancoder.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cuda-huffmancoder.dir/build: cuda-huffmancoder
.PHONY : CMakeFiles/cuda-huffmancoder.dir/build

CMakeFiles/cuda-huffmancoder.dir/requires: CMakeFiles/cuda-huffmancoder.dir/main.cpp.o.requires
.PHONY : CMakeFiles/cuda-huffmancoder.dir/requires

CMakeFiles/cuda-huffmancoder.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cuda-huffmancoder.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cuda-huffmancoder.dir/clean

CMakeFiles/cuda-huffmancoder.dir/depend:
	cd /home/bhugo/brandon/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bhugo/brandon/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder /home/bhugo/brandon/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder /home/bhugo/brandon/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/build /home/bhugo/brandon/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/build /home/bhugo/brandon/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/build/CMakeFiles/cuda-huffmancoder.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cuda-huffmancoder.dir/depend

