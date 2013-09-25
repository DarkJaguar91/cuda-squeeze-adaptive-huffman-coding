/*
 * GrapicsCompression.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: bjtal
 */

#include "GrapicsCompression.h"

void GPU::binVector(thrust::device_vector<float> & dev_vec,
		frequencyValues & map, double & time) {
	Timer::tic();

	thrust::sort(dev_vec.begin(), dev_vec.end());

	int num_bins = thrust::inner_product(dev_vec.begin(), dev_vec.end() - 1,
			dev_vec.begin() + 1, int(1), thrust::plus<int>(),
			thrust::not_equal_to<float>());

	thrust::device_vector<float> unique_device(num_bins);
	thrust::device_vector<int> counts_device(num_bins);

	thrust::reduce_by_key(dev_vec.begin(), dev_vec.end(),
			thrust::constant_iterator<int>(1), unique_device.begin(),
			counts_device.begin());
	printf("Binning: %f\n", Timer::toc());
	time += Timer::toc();

	Timer::tic();
	thrust::host_vector<float> unique_host = unique_device;
	thrust::host_vector<int> counts_host = counts_device;
	printf("Device to host copy (counts & unique): %f\n", Timer::toc());
	time += Timer::toc();

	Timer::tic();
	for (int i = 0; i < num_bins; ++i)
		map[unique_host[i]] = new longValue(counts_host[i]);
	printf("Conversion to HashMap: %f\n", Timer::toc());
	time += Timer::toc();
}

void GPU::compress(const longValue & numberOfFloats) {
	thrust::host_vector<float> h_vec(numberOfFloats);

	srand(time(NULL));

	for (longValue i = 0; i < numberOfFloats; ++i)
		h_vec[i] = rand() % 9000;

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
	thrust::device_vector<float> dev_vec = h_vec;
	printf("\e[0;33mCopy Time (not-included): %f\n\e[0m", Timer::toc());
	//	time += Timer::toc();

	// bin data
	binVector(dev_vec, map, totTime);

	// generate the tree
	Timer::tic();
	comp.initialize();
	printf("Tree Creation: %f\n", Timer::toc());
	totTime += Timer::toc();

	// CPU copy data from map to new values
	Timer::tic();
	longValue numProcess = ceil(numberOfFloats / (numThreads));
#pragma omp parallel for num_threads(numThreads)
	for (longValue i = 0; i < longValue((numThreads)); ++i) {

		longValue proc = min(numberOfFloats - (i * numProcess), numProcess);

		comp.compress(&h_vec[0] + (i * numProcess), codes + (i * numProcess),
				proc);
	}
	printf("Swapping Values: %f\n", Timer::toc());
	totTime += Timer::toc();

	printf("\e[1;32mTotal Time: %f\n\e[0m", totTime);
}
