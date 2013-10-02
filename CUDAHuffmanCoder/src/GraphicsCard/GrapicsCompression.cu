/*
 * GrapicsCompression.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: bjtal
 */

#include "GrapicsCompression.h"

__global__ void swapCodes(float * data, float * codeData, const int numData,
		const int numCodeData) {
	int pos = threadIdx.x + threadIdx.y * blockDim.x + blockIdx.x
			+ blockIdx.y * gridDim.x;

	if (pos < numData)
		for (int i = 0; i < numCodeData; ++i) {
//			printf("%f - %f\n", data[pos], codeData[i]);
			if (data[pos] == codeData[i]) {
				data[pos] = i;
				break;
			}
		}
}

void GPU::compress(const longValue & numberOfFloats) {
	srand(time(NULL));
	host_vec host(numberOfFloats);
	for (longValue i = 0; i < numberOfFloats; ++i) {
		host[i] = rand() % 9000;
	}

	int numThreads = omp_get_max_threads();

	printf("\e[0;33mUsing: %d threads\n\e[0m", omp_get_max_threads());

	double totTime = 0;

	Timer::tic();
	HuffCode ** codes;
	codes = (HuffCode **) malloc(sizeof(HuffCode*) * numberOfFloats);
	frequencyValues map;
	Compressor comp(map);
	printf("Pre Processing: %f\n", Timer::toc());
	totTime += Timer::toc();

	// copy Data - Not adding to tot time for now
	Timer::tic();
	dev_vec dev = host;
	printf("\e[0;33mCopy Time (not-included): %f\n\e[0m", Timer::toc());
	//	time += Timer::toc();

	// sort data
	Timer::tic();
	thrust::sort(dev.begin(), dev.end());
	printf("sort: %f\n", Timer::toc());
	totTime += Timer::toc();

	// bin data
	Timer::tic();
	int num_bins = thrust::inner_product(dev.begin(), dev.end() - 1,
			dev.begin() + 1, int(1), thrust::plus<int>(),
			thrust::not_equal_to<float>());

	dev_vec unique_device(num_bins);
	dev_vec_int counts_device(num_bins);

	thrust::reduce_by_key(dev.begin(), dev.end(),
			thrust::constant_iterator<int>(1), unique_device.begin(),
			counts_device.begin());
	printf("Binning: %f\n", Timer::toc());
	totTime += Timer::toc();

	Timer::tic();
	host_vec unique_host = unique_device;
	host_vec_int counts_host = counts_device;
	printf("Device to host copy (counts & unique): %f\n", Timer::toc());
	totTime += Timer::toc();

	Timer::tic();
	for (int i = 0; i < num_bins; ++i)
		map[unique_host[i]] = new longValue(counts_host[i]);
	// some memory management Remove counts as we dont need them
	counts_device.clear();
	counts_device.shrink_to_fit();
	counts_host.clear();
	counts_host.shrink_to_fit();
	unique_host.clear(); // clear this as its duplicated
	unique_host.shrink_to_fit();

	printf("Conversion to HashMap: %f\n", Timer::toc());
	totTime += Timer::toc();

	// generate the tree
	Timer::tic();
	comp.initialize();
	printf("Tree Creation: %f\n", Timer::toc());
	totTime += Timer::toc();

//	// swap data GPU -->
//	Timer::tic();
//	dev = host;
////	thrust::host_vector<int> length(num_bins);
////	thrust::host_vector<int> startPos(num_bins);
////	thrust::host_vector<bool> codeArr(comp.getTotalCodeLength());
////	int pos = 0;
////	for (int i = 0; i < num_bins; ++i) {
////		HuffCode code = *(comp.map[unique_host[i]]);
////		length[i] = code.size();
////		startPos[i] = pos;
////		pos += code.size();
////
////		for (int z = 0; z < code.size(); ++z)
////			codeArr[startPos[i] + z] = code[z];
////	}
//	printf("Create array times: %f\n", Timer::toc());
//
//	Timer::tic();
//	dim3 grid, block;
//	block.x = 16;
//	block.y = 16;
//	block.z = 1;
//
//	grid.x = ceil(sqrt((numberOfFloats / (block.x * block.y * block.z))));
//	grid.y = grid.x;
//	grid.z = 1;
//	float * data_ptr = thrust::raw_pointer_cast(dev.data());
//	float * codeData_ptr = thrust::raw_pointer_cast(unique_device.data());
//	swapCodes<<<grid, block>>>(data_ptr, codeData_ptr, numberOfFloats, num_bins);
//	cudaThreadSynchronize();
//	printf("Swap Time: %f\n", Timer::toc());

	// CPU copy data from map to new values
	Timer::tic();
	longValue numProcess = ceil(numberOfFloats / (numThreads));
#pragma omp parallel for num_threads(numThreads)
	for (longValue i = 0; i < longValue((numThreads)); ++i) {

		longValue proc = min(numberOfFloats - (i * numProcess), numProcess);

		comp.compress(&host[0] + (i * numProcess), codes + (i * numProcess),
				proc);
	}
	printf("Swapping Values: %f\n", Timer::toc());
	totTime += Timer::toc();

	printf("\e[1;32mTotal Time: %f\n\e[0m", totTime);
}
