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
#include <fstream>

#include "GraphicsCard/GrapicsCompression.h"
#include "CPU/CPUmain.h"

int main() {
	using namespace std;

	size_t freeSize, totSize;
	cudaMemGetInfo(&freeSize, &totSize);

	printf("Free Memory: %.2fMB | Total Memory: %.2fMB\n\n",
			(freeSize / 1024 / 1024.0f), (totSize / 1024 / 1024.0f));

	longValue numFloats = 216 * 4096 * 112 * 2;
//	numFloats *= 1.0f;
//	numFloats /= sizeof(float);

//	longValue numFloats = floor((freeSize) * 0.8f / sizeof(float));
	printf("Allocating %.2fMB of floats\n\n",
			(numFloats * 4.0f / 1024.0f / 1024.0f));
//
//	// make file
////	FILE * out;
////	out = fopen("input.values", "w");
////	srand(time(NULL));
////	fwrite(&numFloats, sizeof(longValue), 1, out);
////	for (longValue i = 0; i < numFloats; ++i){
////		float f = rand() % 9000;
////		fwrite(&f, sizeof(float), 1, out);
////	}
////	fclose(out);
//	ofstream out;
//	out.open("input.values2");
//	out << numFloats;
//	for (longValue i = 0; i < numFloats; ++i){
//		float f = rand() % 9000;
//		out << " " << f;
//	}
//	out.close();
//
//	printf("Created file\n\n");

//	printf("CPU Version\n");
//	printf("--------------------------------------------------------------\n");
//	CPUCode::compressCPU(numFloats);
//	printf("--------------------------------------------------------------\n\n");

	printf("GPU Version\n");
	printf("--------------------------------------------------------------\n");
	omp_set_num_threads(1);
	GPU::compress(numFloats);
	printf("\n");
	omp_set_num_threads(4);
	GPU::compress(numFloats);
	printf("\n");
	omp_set_num_threads(8);
	GPU::compress(numFloats);
	printf("\n");
	omp_set_num_threads(16);
	GPU::compress(numFloats);
	printf("\n");
	printf("--------------------------------------------------------------\n");

	return 0;
}
