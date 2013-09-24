#include <iostream>
#include <vector>
#include <time.h>

#include "CPUCompression/CPUmain.h"
#include "GPUCompressor/gpurunner.h"

int main(int argc, char **argv) {
    using namespace std;
    

	longValue numFloats = 1024 * 1024 * 800 / sizeof(float);
	numFloats *= 1;
	

	cout << "CPU Version" << endl;
        CPUCode::compressCPU(numFloats);
    
        cout << "GPU Version" << endl;
        GPUCoder::compressGPU(numFloats);
    
    return 0;
}
