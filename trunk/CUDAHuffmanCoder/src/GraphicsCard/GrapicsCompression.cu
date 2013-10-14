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
	frequencyValues map;
	std::vector<unsigned char> codes;
	double totTime = 0;
	compressData(host, map, codes, totTime);
}

void GPU::compress(char * inputFileName, char * outputFileName) {
	Timer::tic();
	FILE * in;
	in = fopen(inputFileName, "r");
	longValue numFloats;
	size_t res = fread(&numFloats, sizeof(longValue), 1, in);
	host_vec host;
	host.resize(numFloats);
	res = fread(&host[0], sizeof(float), numFloats, in);
	fclose(in);
	double readTime = Timer::toc();
	printf("Time to read data: %f\n", readTime);

	std::vector<unsigned char> charArray;
	frequencyValues map;
	double processTime = 0;
	compressData(host, map, charArray, processTime);

	Timer::tic();
	FILE * out;
	out = fopen(outputFileName, "w");

	longValue numUnique = map.size();
	fwrite(&numUnique, sizeof(longValue), 1, out);

	for (frequencyValues::iterator it = map.begin(); it != map.end(); ++it){
		float uval = it->first;
		longValue cnt = *it->second;

		fwrite(&uval, sizeof(float), 1, out);
		fwrite(&cnt, sizeof(longValue), 1, out);
	}

	longValue numChars = charArray.size();
	fwrite(&numChars, sizeof(longValue), 1, out);
	fwrite(&charArray[0], sizeof(unsigned char), numChars, out);

	fclose(out);
	double writeTime = Timer::toc();
	printf("Time to write data: %f\n", writeTime);

	printf("Total Time: %f\n", writeTime + readTime + processTime);
}

void GPU::compressData(host_vec & host, frequencyValues & map, std::vector<unsigned char> & charCodes, double & totTime){
	int numThreads = omp_get_max_threads();
	longValue numberOfFloats = host.size();

	printf("Using: %d threads\n", omp_get_max_threads());

	Timer::tic();
	HuffCode ** codes;
	codes = (HuffCode **) malloc(sizeof(HuffCode*) * numberOfFloats);
	Compressor comp(map);
	printf("Pre Processing: %f\n", Timer::toc());
	totTime += Timer::toc();

	// copy Data - Not adding to tot time for now
	Timer::tic();
	dev_vec dev = host;
	printf("Copy Time (included): %f\n", Timer::toc());
	totTime += Timer::toc();

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

	printf("Process before char creation: %f\n", totTime);

	Timer::tic();
	///////////////////////////
	longValue cnt = 0;
	double size = 0;

	unsigned char b = 0;
	for (longValue i = 0; i < numberOfFloats; ++i) {
		for (int z = 0; z < codes[i]->size(); ++z) {
			if (codes[i]->at(z) == 1)
				b |= (1 << (7 - (cnt % 8)));
			else
				b &= ~(1 << (7 - (cnt % 8)));
			cnt++;
			if (cnt % 8 == 0)
			{
				charCodes.push_back(b);
				b = 0;
			}
			size++;
		}
	}
	///////////////////////////////////////
	///////////////////////////////////////
//	longValue cnt = 0;
//	for (longValue i = 0; i < numberOfFloats; ++i) {
//		cnt += codes[i]->size();
//	}
//
//	double size = cnt;
//
//	HuffCode array(cnt);
//
//
//	cnt = 0;
//	for (longValue i = 0; i < numberOfFloats; ++i) {
//		for (int z = 0; z < codes[i]->size(); ++z) {
//			array[cnt] = codes[i]->at(z);
//			cnt++;
//		}
//	}
//	printf("Flattening array: %f\n", Timer::toc());
//	totTime += Timer::toc();
//
//	Timer::tic();
//	int numChars = int(ceil(array.size() / float(8)));
//	charCodes.resize(numChars);
//	int i = 0;
//#pragma omp parallel for private(i) schedule(dynamic) shared(array)
//	for (i = 0; i < numChars; ++i) {
//		unsigned char b = 0;
//		for (int z = 0; z < 8; ++z) {
//			if (array[i * 8 + z] == 1)
//				b |= (1 << (7 - (z % 8)));
//			else
//				b &= ~(1 << (7 - (z % 8)));
//		}
//		charCodes[i] = b;
//	}
	////////////////////////////////////////////////////////////////////
	printf("Converting to char: %f\n", Timer::toc());
	totTime += Timer::toc();

	printf("Total Time for processing: %f\n", totTime);

	size = ceil(size / 8.0f) * sizeof(unsigned char);
	size += map.size() * sizeof(float) + map.size() * sizeof(longValue) + sizeof(longValue) * 2;
	double inputSize = float(numberOfFloats * sizeof(float) + sizeof(longValue));
	printf("Size of output file: %f\n", size / 1024.0f / 1024.0f);
	printf("Size of input file: %f\n", inputSize / 1024.0f / 1024.0f);
	printf("compression ratio: %f\n", size / inputSize);

	delete [] codes;

	/// check
	Timer::tic();
	Decompressor decomp(map);
	decomp.initialize();
	std::vector<float> floats;

	HuffCode array;
	for (int i = 0; i < charCodes.size(); ++i){
		unsigned char c = charCodes.at(i);
		for (long i = 0; i < 8; ++i) {
			array.push_back(((c >> (7 - i)) & 1));
		}
	}
	decomp.decode(array, floats);
	printf("Timer to decompress: %f\n", Timer::toc());

	for (int i = 0; i < floats.size(); ++i)
		if (floats[i] != host[i])
			printf("failed %d->   %f   <>   %f\n", i, floats[i], host[i]);
}
