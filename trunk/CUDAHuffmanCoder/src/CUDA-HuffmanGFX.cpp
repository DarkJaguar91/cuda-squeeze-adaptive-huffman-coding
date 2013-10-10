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

	printf("Free Memory: %.2fMB | Total Memory: %.2fMB\n\n",
			(freeSize / 1024 / 1024.0f), (totSize / 1024 / 1024.0f));

//	longValue numFloats = 1024 * 1024 * 1024;
//	numFloats *= 0.5f;
//	numFloats /= sizeof(float);

	longValue numFloats = floor((freeSize) * 0.48f / sizeof(float));
	printf("Allocating %.2fMB of floats\n\n", (numFloats * 4.0f / 1024.0f / 1024.0f));

	// make file
	FILE * out;
	out = fopen("input.values", "w");
	srand(time(NULL));
	fwrite(&numFloats, sizeof(longValue), 1, out);
	for (longValue i = 0; i < numFloats; ++i){
		float f = rand() % 9000;
		fwrite(&f, sizeof(float), 1, out);
	}
	fclose(out);
	printf("Created file\n\n");

	printf("CPU Version\n");
	printf("--------------------------------------------------------------\n");
	CPUCode::compressCPU(numFloats);
	printf("--------------------------------------------------------------\n\n");

	printf("GPU Version\n");
	printf("--------------------------------------------------------------\n");
	GPU::compress("input.values", "outputGPU.hc");
	printf("--------------------------------------------------------------\n");

	return 0;
}
