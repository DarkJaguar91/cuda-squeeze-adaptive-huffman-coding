//============================================================================
// Name        : CSAHC.cpp
// Author      : Brandon James Talbot
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include "HuffmanCoder/HTree.h"

int main() {
	using namespace HuffmanCoder;
	using namespace std;

	HTree t;

	t.addData(10);
	t.addData(12);
	t.addData(100);
	t.addData(10);
	t.addData(-100.99);

	t.print();

	return 0;
}
