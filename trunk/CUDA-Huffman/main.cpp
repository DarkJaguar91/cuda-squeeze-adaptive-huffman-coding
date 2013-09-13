#include <iostream>

#include "CPUCompression/CPUmain.h"
#include "GPUCompression/GPUmain.h"

int main(int argc, char **argv) {
    //GPUCode::compressGPU();
    CPUCode::compressCPU();
    
    GPUCode::compressGPU();
    return 0;
}