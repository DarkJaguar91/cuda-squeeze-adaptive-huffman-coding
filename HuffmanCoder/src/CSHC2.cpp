#include <iostream>
#include <algorithm>
#include <math.h>
#include <time.h>
#include <cstdio>

#include "HuffmanCoder/Compressor.h"
#include "HuffmanCoder/Decompressor.h"
#include "Timer/Timer.h"

int main(int argc, char *argv[]) {
	using namespace std;

	int numThreads = 4;

	if (argc >= 2)
		numThreads = atoi(argv[1]);

	srand(time(NULL));

	omp_set_num_threads(numThreads);
	cout << "Number of threads: " << omp_get_max_threads() << endl;

	vector<float> floats;
	for (longValue i = 0; i < 1024 * 1024 * 100 / 4; ++i)
		floats.push_back(rand() % 9000);

	frequencyValues map;

	Compressor comp(map);
	vector<HuffCode *> codes;

	cout << "Compressing - ";
	flush(cout);
	Timer::tic();
	comp.initialize(floats);
	comp.compress(floats, codes);
	cout << "Done in: " << Timer::toc() << endl;

	Decompressor decomp(map);

	// convert code to one array
	HuffCode code;
	for (vector<HuffCode *>::iterator it = codes.begin(); it != codes.end();
			++it)
		std::copy((*it)->begin(), (*it)->end(), std::back_inserter(code));

	vector<float> ff;

	cout << "Decompressing - ";
	flush(cout);
	Timer::tic();
	decomp.initialize();
	decomp.decode(code, ff);
	cout << "Done in: " << Timer::toc() << endl;

	cout << "Checking data - ";
	flush(cout);
	for (longValue i = 0; i < 1024 * 1024 * 100 / 4; ++i) {
		if (ff[i] != floats[i]) {
			cout << "FAILED!!!\n";
			return 0;
		}
	}
	cout << "PASSED!!!\n";

	return 0;
}
