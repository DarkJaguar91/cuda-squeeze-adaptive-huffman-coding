#include <iostream>
#include <vector>
#include <time.h>

#include "CPUCompression/CPUmain.h"
#include "GPUCompression/gpumain.h"

int main(int argc, char **argv) {
    using namespace std;
    
    cout << "CPU Version" << endl;
    CPUCode::compressCPU();
    
    //cout << "GPU Version" << endl;
    //GPUCode::compressGPU();
    
    return 0;
}
