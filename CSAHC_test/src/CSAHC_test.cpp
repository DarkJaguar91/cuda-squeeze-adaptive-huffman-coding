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

int main() {


	HTree adh;
	srand(time(NULL));

	Timer::tic();
	for (int i = 0; i < 3; ++i)
		adh.encodeValue(rand() % 5);


	std::cout << "Finished in: " << Timer::toc() << std::endl;;
	std::flush(std::cout);
	adh.print();

	return 0;
}
