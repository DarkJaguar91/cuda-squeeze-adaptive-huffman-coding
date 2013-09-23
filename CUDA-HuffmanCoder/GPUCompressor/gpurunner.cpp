#include "gpurunner.h"

#include "../GPUCompression/gpumain.h"

void GPUCoder::compressGPU(){
	using namespace std;
	int numThreads = omp_get_max_threads();

	//omp_set_num_threads(numThreads);
	cout << "Using: " << omp_get_max_threads() << " threads\n";

	longValue numFloats = 1024 * 1024 * 1 / sizeof(float);
	numFloats *= 1;
	
	srand(time(NULL));
	std::vector<float> floats(numFloats);

	for (longValue i = 0; i < numFloats; ++i) {
		floats[i] = rand() % 9000;
	}

	// Compression
	HuffCode ** codes;
	codes = (HuffCode **) malloc(sizeof(HuffCode*) * numFloats);
	frequencyValues map;

	Timer::tic();
	Compressor comp(map);
	double time = Timer::toc();
	time += GPUCode::compressGPUlib(&floats[0], map, numFloats);
	Timer::tic();
	comp.initialize();
	
	longValue numProcess = ceil(numFloats / (numThreads));
#pragma omp parallel for num_threads(numThreads)
	for (longValue i = 0; i < longValue((numThreads)); ++i) {
		
		longValue proc = min(numFloats - (i * numProcess), numProcess);
		
		comp.compress(&floats[0] + (i * numProcess), codes + (i * numProcess), proc);
	}
	time += Timer::toc();
	cout << "Compression took: " << time << endl;	
	

	delete[] codes;
}
