project(cuda-huffman)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)
#declare executable
find_package(CUDA)
cuda_add_executable(cuda-huffman main.cpp)
install(TARGETS cuda-huffman RUNTIME DESTINATION bin)

add_subdirectory(GPUCompression)
add_subdirectory(CPUCompression)
add_subdirectory(Timer)

target_link_libraries(cuda-huffman gpuCode)
target_link_libraries(cuda-huffman cpuCode)
target_link_libraries(cuda-huffman timerLib)

find_package(rt)
target_link_libraries(cuda-huffman rt)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()
target_link_libraries(cuda-huffman gomp)

set(CMAKE_BUILD_TYPE Release)
set( CMAKE_CXX_FLAGS "-Wall -m64 -O3 -fopenmp -fno-stack-protector" )