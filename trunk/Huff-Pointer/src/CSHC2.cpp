#include <iostream>
#include <algorithm>
#include <math.h>
#include <time.h>
#include <cstdio>
#include <stdint.h>

#include "HuffmanCoder/Compressor.h"
#include "HuffmanCoder/Decompressor.h"
#include "Timer/Timer.h"

int main(int argc, char *argv[]) {
	using namespace std;

	int numThreads = 2;

	if (argc >= 2)
		numThreads = atoi(argv[1]);

	omp_set_num_threads(8);
	cout << "Using: " << omp_get_max_threads() << " threads\n";

	longValue numFloats = 1024 * 1024 * 1024 / sizeof(float);

	srand(time(NULL));
	float * floats = new float[numFloats];
	for (longValue i = 0; i < numFloats; ++i) {
		floats[i] = rand() % 9000;
	}
	HuffCode code;
	frequencyValues map;

	// Compression
	HuffCode ** codes = new HuffCode*[numFloats];
	Compressor comp(map);

	Timer::tic();
	comp.initialize(floats, numFloats);
	comp.compress(floats, codes, numFloats);

	//	longValue numProcess = ceil(numFloats / 100);
	//#pragma omp parallel for
	//	for (longValue i = 0; i < 100; ++i){
	//		longValue proc = min(numFloats - (i*numProcess), numProcess);
	//		comp.compress(floats+(i*numProcess), codes+(i*numProcess), proc);
	//	}
	cout << "Compression took: " << Timer::toc() << endl;

	for (longValue i = 0; i < numFloats; ++i) {
		std::copy(codes[i]->begin(), codes[i]->end(), std::back_inserter(code));
	}

	delete[] codes;

	// Decompression
	Decompressor decomp(map);
	vector<float> floatsvec;
	Timer::tic();
	decomp.initialize();
	decomp.decode(code, floatsvec);
	cout << "Decompression took: " << Timer::toc() << endl;

	for (longValue i = 0; i < numFloats; ++i){
		if (floats[i] != floatsvec[i]){
			cout <<"Failed!\n";
			break;
		}
	}

	delete[] floats;

	return 0;
}
