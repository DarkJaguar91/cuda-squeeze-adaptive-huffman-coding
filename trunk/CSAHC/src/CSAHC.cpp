//============================================================================
// Name        : CSAHC.cpp
// Author      : Brandon James Talbot
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>

#include "AdaptiveHuffmanCoder/Tree.h"

int main() {
	using namespace std;

//	vector< int * > ints;
//
//	ints.push_back(new int(1));
//
//	cout << ints.size() << endl;

	HTree<float> t(255);

	t.encode(1);
	t.encode(2);
	t.encode(3);
	t.encode(3);
	t.encode(4);
//	t.encode(3);

	t.print();

	return 0;
}
