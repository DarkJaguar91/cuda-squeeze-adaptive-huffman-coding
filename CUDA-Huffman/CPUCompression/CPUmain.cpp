#include "CPUmain.h"

void CPUCode::compressCPU(){
  using namespace std;

	int numThreads = omp_get_max_threads();

	omp_set_num_threads(numThreads);
	cout << "Using: " << omp_get_max_threads() << " threads\n";

	longValue numFloats = 1024 * 1024 * 1024 / sizeof(float);

	srand(time(NULL));
	float * floats;//= new float[numFloats];

	floats = (float*) malloc(sizeof(float) * numFloats);
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

		comp.initialize(floats + (i * numProcess), proc);
		comp.compress(floats + (i * numProcess), codes + (i * numProcess), proc);
	}
	cout << "Compression took: " << Timer::toc() << endl;

	delete[] codes;
	delete[] floats;
}