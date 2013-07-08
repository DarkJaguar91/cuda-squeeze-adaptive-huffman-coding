/*
 * runner.cpp
 *
 *  Created on: 08 Jul 2013
 *      Author: root
 */


#include "AdaptiveHuffmanCoder/HTree.h"
#include <time.h>
#include "Timer/Timer.h"

int main (){
	HTree adh;
	srand(time(NULL));

	Timer::tic();
	for (int i = 0; i < 1024 * 1024; ++i)
		adh.encodeValue(rand() % 255);


	std::cout << "Finished in: " << Timer::toc() << std::endl;;
	std::flush(std::cout);
	adh.print();
}
