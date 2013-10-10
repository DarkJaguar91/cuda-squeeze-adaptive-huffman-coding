#include "CPUmain.h"

void CPUCode::compressCPU(longValue numFloats) {
	using namespace std;
	int numThreads = omp_get_max_threads();

	//omp_set_num_threads(numThreads);
	printf("Using: %d threads\n", omp_get_max_threads());

	srand(time(NULL));
	std::vector<float> floats(numFloats);

	for (longValue i = 0; i < numFloats; ++i) {
		floats[i] = rand() % 9000;
	}

	std::vector<unsigned char> charArray;
	frequencyValues * map = new frequencyValues[numThreads];
	compressCPUData(floats, map, charArray, numThreads);
}

void CPUCode::compressCPUData(std::vector<float> & floats,
		frequencyValues * map, std::vector<unsigned char> & charCodes,
		int & numThreads) {
	using namespace std;
	longValue numFloats = floats.size();

	// Compression
	HuffCode ** codes;
	codes = (HuffCode **) malloc(sizeof(HuffCode*) * numFloats);

	Timer::tic();
	longValue numProcess = ceil(numFloats / (numThreads));
#pragma omp parallel for num_threads(numThreads)
	for (longValue i = 0; i < longValue((numThreads)); ++i) {
		Compressor comp(map[i]);
		longValue proc = min(numFloats - (i * numProcess), numProcess);

		comp.initialize(&floats[0] + (i * numProcess), proc);
		comp.compress(&floats[0] + (i * numProcess), codes + (i * numProcess),
				proc);
	}

	longValue cnt = 0;
	for (longValue i = 0; i < numFloats; ++i) {
		cnt += codes[i]->size();
	}

	double size = ceil(cnt / 8.0f) * sizeof(unsigned char);
	for (int i = 0; i < numThreads; ++i)
		size += sizeof(float) * map[i].size()
				+ sizeof(longValue) * map[i].size();
	size += sizeof(longValue) * 2;
	double inputSize = float(numFloats * sizeof(float) + sizeof(longValue));
	printf("Size of output file: %f\n", size / 1024.0f / 1024.0f);
	printf("Size of input file: %f\n", inputSize / 1024.0f / 1024.0f);
	printf("compression ratio: %f\n", size / inputSize);

//	HuffCode arr(cnt);
//	cnt = 0;
//	for (longValue i = 0; i < numFloats; ++i) {
//		for (int z = 0; z < codes[i]->size(); ++z) {
//			arr[cnt] = codes[i]->at(z);
//			cnt++;
//		}
//	}

//	int numChars = int(ceil(array.size() / 8.0f));
//	charCodes.resize(numChars);
//#pragma omp parallel for
//	for (int i = 0; i < numChars; ++i) {
//		unsigned char b = 0;
//		for (int z = 0; z < 8; ++z) {
//			if (array[i * 8 + z] == 1)
//				b |= (1 << (7 - (z % 8)));
//			else
//				b &= ~(1 << (7 - (z % 8)));
//		}
//		charCodes[i] = b;
//	}

	printf("Compression took: %f\n", Timer::toc());

	delete[] codes;
}
