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

#include "GraphicsCard/GrapicsCompression.h"

int main()
{
	using namespace std;

	longValue numFloats = 1024 * 1024 * 1024;
	numFloats *= 1.3f;
	numFloats /= sizeof(float);

	cout << (numFloats * 4 / 1024 / 1024) << " MB\n";

	cout << "GPU Version:" << endl;
	GPU::compress(numFloats);


	return 0;
}
