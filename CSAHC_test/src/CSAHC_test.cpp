//============================================================================
// Name        : CSAHC_test.cpp
// Author      : Brandon James talbot
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

#include "AdaptiveHuffmanCoder/HTree.h"
#include "Timer/Timer.h"

void addValue(std::vector<byte> code, std::vector<byte> & out) {
	using namespace std;

	for (int i = 0; i < code.size(); ++i) {
//		cout << (code.at(i) == 0 ? "0" : "1");
		out.push_back(code.at(i));
	}

//	cout << endl;
}

int main() {
	using namespace std;

	vector<float> randVals;
	vector<byte> code;

	HTree adh;
	srand(time(NULL));

	Timer::tic();
	for (int i = 0; i < 1024; ++i) {
		float f = rand() % 255;
		randVals.push_back(f);
		addValue(adh.encodeValue(f), code);
	}

	std::cout << "Finished in: " << Timer::toc() << std::endl;

	std::flush(std::cout);
//	adh.print();

	vector<float> uniqueVals = adh.getUniqueVals();

	cout << uniqueVals.size() << endl;
//	cout << code.size() << endl;
	int numVals = adh.getNumberValues();
	cout << numVals << endl;

	std::cout << "\n\nDecoding:\n";
	Timer::tic();

	HTree adh2(uniqueVals);

	vector<float> output = adh2.decode(code, numVals);

	cout << "decoded in: " << Timer::toc() << endl;

//	adh2.print();

	if (output.size() != randVals.size())
	{
		cout << "Not even correct size...\n";
		return 0;
	}

	for (int i = 0; i < output.size(); ++i)
	if (output.at(i) != randVals.at(i))
		cout << randVals.at(i) << " <> " << output.at(i) << endl;

	return 0;
}
