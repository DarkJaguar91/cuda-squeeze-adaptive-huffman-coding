CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

FIND_PACKAGE(CUDA)

if(CUDA_FOUND)

    SET(CUDA_NVCC_FLAGS "-arch;sm_20")
    #set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-gencode arch=compute_20,code=sm_20)

else(CUDA_FOUND)

    message("CUDA is not installed on this system.")

endif()

SET(LIB_TYPE STATIC)	#set the lib type

CUDA_ADD_LIBRARY(gpuLib ${LIB_TYPE} GPUlib.cu)