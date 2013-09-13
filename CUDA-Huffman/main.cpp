#include <iostream>

#include "GPUCompression/GPUmain.h"
#include "CPUCompression/CPUmain.h"

int main(int argc, char **argv) {
    //GPUCode::compressGPU();
    CPUCode::compressCPU();
    return 0;
}