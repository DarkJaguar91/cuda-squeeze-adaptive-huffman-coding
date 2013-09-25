#include "CPUmain.h"

void CPUCode::compressCPU(longValue numFloats){
  using namespace std;
	int numThreads = omp_get_max_threads();

	//omp_set_num_threads(numThreads);
	printf("\e[0;33mUsing: %d threads\n\e[0m", omp_get_max_threads());

	srand(time(NULL));
	std::vector<float> floats(numFloats);

	for (longValue i = 0; i < numFloats; ++i) {
		floats[i] = rand() % 9000;
	}

	// Compression
	HuffCode ** codes;
	codes = (HuffCode **) malloc(sizeof(HuffCode*) * numFloats);
	frequencyValues * map = new frequencyValues[numThreads];

	Timer::tic();
	longValue numProcess = ceil(numFloats / (numThreads));
#pragma omp parallel for num_threads(numThreads)
	for (longValue i = 0; i < longValue((numThreads)); ++i) {
		Compressor comp(map[i]);
		longValue proc = min(numFloats - (i * numProcess), numProcess);

		comp.initialize(&floats[0] + (i * numProcess), proc);
		comp.compress(&floats[0] + (i * numProcess), codes + (i * numProcess), proc);
	}
	printf("\e[1;32mCompression took: %f\n\e[0m", Timer::toc());

	delete[] codes;
}
