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

#include "GraphicsCard/GrapicsCompression.h"
#include "CPU/CPUmain.h"

int main()
{
	using namespace std;

	size_t freeSize, totSize;
	cudaMemGetInfo(&freeSize, &totSize);
	
	cout << "Free Memory: " << (freeSize / 1024 / 1024.0f) << " | " << " Total Memory: " << (totSize / 1024 / 1024.0f) << endl << endl;

//	longValue numFloats = 1024 * 1024 * 1024;
//	numFloats *= 0.5f;
//	numFloats /= sizeof(float);

	longValue numFloats = floor((freeSize) / 2.2f / sizeof(float));


	cout << (numFloats * 4 / 1024 / 1024) << " MB\n\n";

	cout << "CPU Version:" << endl;
	CPUCode::compressCPU(numFloats);

	cout << endl;

	cout << "GPU Version:" << endl;
	GPU::compress(numFloats);


	return 0;
}
