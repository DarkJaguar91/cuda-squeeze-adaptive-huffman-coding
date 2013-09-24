project(cuda-huffmancoder)

cmake_minimum_required(VERSION 2.8)

find_package(CUDA)

if(CUDA_FOUND)
    SET(CUDA_NVCC_FLAGS "-O3 -Xcompiler -fopenmp -gencode arch=compute_20,code=sm_20 -gencode arch=compute_20,code=sm_21 -gencode arch=compute_30,code=sm_30 -gencode arch=compute_35,code=sm_35")
    #set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-Xcompiler -openmp -gencode arch=compute_20,code=sm_20)
else(CUDA_FOUND)
    message("CUDA is not installed on this system.")
endif()

cuda_add_executable(cuda-huffmancoder CUDA-HuffmanGFX.cpp)

install(TARGETS cuda-huffmancoder RUNTIME DESTINATION bin)

add_subdirectory(GraphicsCard)
add_subdirectory(Timer)

target_link_libraries(cuda-huffmancoder gpuCode)
target_link_libraries(cuda-huffmancoder timerLib)

#find_package(rt)
#target_link_libraries(cuda-huffman rt)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()
target_link_libraries(cuda-huffmancoder gomp)


set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-O3 -Wall -m64 -O3 -fopenmp -fno-stack-protector" )
