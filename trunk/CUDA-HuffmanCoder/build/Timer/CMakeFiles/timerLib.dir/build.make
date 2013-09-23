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
CMAKE_SOURCE_DIR = /home/bjtal/workspace/Hons/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bjtal/workspace/Hons/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/build

# Include any dependencies generated for this target.
include Timer/CMakeFiles/timerLib.dir/depend.make

# Include the progress variables for this target.
include Timer/CMakeFiles/timerLib.dir/progress.make

# Include the compile flags for this target's objects.
include Timer/CMakeFiles/timerLib.dir/flags.make

Timer/CMakeFiles/timerLib.dir/Timer.cpp.o: Timer/CMakeFiles/timerLib.dir/flags.make
Timer/CMakeFiles/timerLib.dir/Timer.cpp.o: ../Timer/Timer.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/bjtal/workspace/Hons/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Timer/CMakeFiles/timerLib.dir/Timer.cpp.o"
	cd /home/bjtal/workspace/Hons/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/build/Timer && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/timerLib.dir/Timer.cpp.o -c /home/bjtal/workspace/Hons/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/Timer/Timer.cpp

Timer/CMakeFiles/timerLib.dir/Timer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/timerLib.dir/Timer.cpp.i"
	cd /home/bjtal/workspace/Hons/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/build/Timer && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/bjtal/workspace/Hons/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/Timer/Timer.cpp > CMakeFiles/timerLib.dir/Timer.cpp.i

Timer/CMakeFiles/timerLib.dir/Timer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/timerLib.dir/Timer.cpp.s"
	cd /home/bjtal/workspace/Hons/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/build/Timer && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/bjtal/workspace/Hons/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/Timer/Timer.cpp -o CMakeFiles/timerLib.dir/Timer.cpp.s

Timer/CMakeFiles/timerLib.dir/Timer.cpp.o.requires:
.PHONY : Timer/CMakeFiles/timerLib.dir/Timer.cpp.o.requires

Timer/CMakeFiles/timerLib.dir/Timer.cpp.o.provides: Timer/CMakeFiles/timerLib.dir/Timer.cpp.o.requires
	$(MAKE) -f Timer/CMakeFiles/timerLib.dir/build.make Timer/CMakeFiles/timerLib.dir/Timer.cpp.o.provides.build
.PHONY : Timer/CMakeFiles/timerLib.dir/Timer.cpp.o.provides

Timer/CMakeFiles/timerLib.dir/Timer.cpp.o.provides.build: Timer/CMakeFiles/timerLib.dir/Timer.cpp.o

# Object files for target timerLib
timerLib_OBJECTS = \
"CMakeFiles/timerLib.dir/Timer.cpp.o"

# External object files for target timerLib
timerLib_EXTERNAL_OBJECTS =

Timer/libtimerLib.a: Timer/CMakeFiles/timerLib.dir/Timer.cpp.o
Timer/libtimerLib.a: Timer/CMakeFiles/timerLib.dir/build.make
Timer/libtimerLib.a: Timer/CMakeFiles/timerLib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libtimerLib.a"
	cd /home/bjtal/workspace/Hons/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/build/Timer && $(CMAKE_COMMAND) -P CMakeFiles/timerLib.dir/cmake_clean_target.cmake
	cd /home/bjtal/workspace/Hons/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/build/Timer && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/timerLib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Timer/CMakeFiles/timerLib.dir/build: Timer/libtimerLib.a
.PHONY : Timer/CMakeFiles/timerLib.dir/build

Timer/CMakeFiles/timerLib.dir/requires: Timer/CMakeFiles/timerLib.dir/Timer.cpp.o.requires
.PHONY : Timer/CMakeFiles/timerLib.dir/requires

Timer/CMakeFiles/timerLib.dir/clean:
	cd /home/bjtal/workspace/Hons/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/build/Timer && $(CMAKE_COMMAND) -P CMakeFiles/timerLib.dir/cmake_clean.cmake
.PHONY : Timer/CMakeFiles/timerLib.dir/clean

Timer/CMakeFiles/timerLib.dir/depend:
	cd /home/bjtal/workspace/Hons/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bjtal/workspace/Hons/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder /home/bjtal/workspace/Hons/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/Timer /home/bjtal/workspace/Hons/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/build /home/bjtal/workspace/Hons/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/build/Timer /home/bjtal/workspace/Hons/cuda-squeeze-adaptive-huffman-coding/CUDA-HuffmanCoder/build/Timer/CMakeFiles/timerLib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Timer/CMakeFiles/timerLib.dir/depend

