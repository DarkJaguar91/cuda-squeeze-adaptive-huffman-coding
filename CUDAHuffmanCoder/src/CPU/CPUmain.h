#include <iostream>
#include <algorithm>
#include <math.h>
#include <time.h>
#include <cstdio>
#include <stdio.h>
#include <stdint.h>
#include <vector>

#include "../HuffmanCoder/Compressor.h"
#include "../HuffmanCoder/Decompressor.h"
#include "../Timer/Timer.h"

namespace CPUCode {
void compressCPU (longValue numFloats);
void compressCPU (char * inputFileName, char * outputFileName);
void compressCPUData (std::vector<float> & floats, frequencyValues * map, std::vector<unsigned char> & charCodes, int & numThreads);
}
