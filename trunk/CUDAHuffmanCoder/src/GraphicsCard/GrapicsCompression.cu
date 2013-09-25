/*
 * GrapicsCompression.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: bjtal
 */

#include "GrapicsCompression.h"

void GPU::binVector(thrust::device_vector<float> & dev_vec, frequencyValues & map,
		double & time) {
	std::cout << "Using: " << omp_get_max_threads() << " threads\n";
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
	std::cout << "Binning Time: " << Timer::toc() << std::endl;
	time += Timer::toc();

	Timer::tic();
	thrust::host_vector<float> unique_host = unique_device;
	thrust::host_vector<int> counts_host = counts_device;
	std::cout << "Unique Value and Count copy to HOST: " << Timer::toc()
			<< std::endl;
	time += Timer::toc();

	Timer::tic();
	for (int i = 0; i < num_bins; ++i)
		map[unique_host[i]] = new longValue(counts_host[i]);
	std::cout << "Conversion to HashMap: " << Timer::toc() << std::endl;
	time += Timer::toc();
}

void GPU::compress(const longValue & numberOfFloats) {
	thrust::host_vector<float> h_vec(numberOfFloats);

	srand(time(NULL));

	for (longValue i = 0; i < numberOfFloats; ++i)
		h_vec[i] = rand() % 9000;

	int numThreads = omp_get_max_threads();

	double totTime = 0;

	Timer::tic();
	HuffCode ** codes;
	codes = (HuffCode **) malloc(sizeof(HuffCode*) * numberOfFloats);
	frequencyValues map;
	Compressor comp(map);
	std::cout << "Pre Processing: " << Timer::toc() << std::endl;
	totTime += Timer::toc();

	// copy Data - Not adding to tot time for now
	Timer::tic();
	thrust::device_vector<float> dev_vec = h_vec;
	std::cout << "Copy Time (not-included): " << Timer::toc() << std::endl;
	//	time += Timer::toc();

	// bin data
	binVector(dev_vec, map, totTime);

	// generate the tree
	Timer::tic();
	comp.initialize();
	std::cout << "Tree creation: " << Timer::toc() << std::endl;
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
	std::cout << "Swapping of values: " << Timer::toc() << std::endl;
	totTime += Timer::toc();

	std::cout << "Total Time: " << totTime << std::endl;
}
