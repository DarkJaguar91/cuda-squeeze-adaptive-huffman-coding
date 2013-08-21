#include <iostream>
#include <algorithm>
#include <math.h>
#include <time.h>

#include "HuffmanCoder/HTree.h"
#include "Timer/Timer.h"

int main() {
	using namespace std;
	vector<float> floats;
	srand(time(NULL));
	for (int i = 0; i < ((1024 * 1024) / sizeof(float)); ++i) {
		floats.push_back(rand() % 50);
	}

	HTreez f;

	HuffCode code;

	Timer::tic();
	f.createCodes(floats, code);
	std::cout << "Time encoding: " << Timer::toc() << std::endl;

	std::flush(std::cout);

//	vector<float> floats2;
//
//	Timer::tic();
//	f.decode(code, floats2);
//	std::cout << "Time decoding: " << Timer::toc() << std::endl;
//
//	if (floats.size() != floats2.size()) {
//		std::cout << "Not same number of floats: " << floats.size() << " - " << floats2.size() << std::endl;
//		return 0;
//	}
//
//	for (int i = 0; i < floats.size(); ++i)
//		if (floats[i] != floats2[i]) {
//			std::cout << "a Number does not match" << std::endl;
//			return 0;
//		}
//
//	std::cout << "Matched" << std::endl;

	return 0;
}
