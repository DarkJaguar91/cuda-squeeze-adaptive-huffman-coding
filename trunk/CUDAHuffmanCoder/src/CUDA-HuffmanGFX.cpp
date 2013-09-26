/**
 * Copyright 1993-2012 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 */
#include <iostream>
#include <math.h>
#include <stdio.h>

#include "GraphicsCard/GrapicsCompression.h"
#include "CPU/CPUmain.h"

int main() {
	using namespace std;

	size_t freeSize, totSize;
	cudaMemGetInfo(&freeSize, &totSize);

	printf("\e[1;31mFree Memory: %.2fMB | Total Memory: %.2fMB\n\n\e[0m", (freeSize / 1024 / 1024.0f), (totSize / 1024 / 1024.0f));

//	longValue numFloats = 1024 * 1024 * 1024;
//	numFloats *= 0.5f;
//	numFloats /= sizeof(float);

	longValue numFloats = floor((freeSize) / 2.2f / sizeof(float));

	printf("\e[1;31mAllocating %.2fMB of floats\n\n\e[0m", (numFloats*4.0f/1024.0f/1024.0f));

	printf("\e[1;36mCPU Version\n\e[0m");
	printf("\e[1;36m--------------------------------------------------------------\n\e[0m");
	CPUCode::compressCPU(numFloats);
	printf("\e[1;36m--------------------------------------------------------------\n\e[0m\n");

	printf("\e[1;36mGPU Version\n\e[0m");
	printf("\e[1;36m--------------------------------------------------------------\n\e[0m");
	GPU::compress(numFloats);
	printf("\e[1;36m--------------------------------------------------------------\n\e[0m");

	return 0;
}
