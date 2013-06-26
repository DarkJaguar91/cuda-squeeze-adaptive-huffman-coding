//============================================================================
// Name        : CSAHC.cpp
// Author      : Brandon James Talbot
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <math.h>


#include "AdaptiveHuffmanCoder/Tree.h"


void printVec(std::vector<int> vec, std::vector<int> & out){
	using namespace std;
	for (vector<int>::iterator it = vec.begin(); it != vec.end(); ++it){
		cout << (*it);
		out.push_back((*it));
	}
	cout << endl;
}

int main() {
	using namespace std;

	std::vector<int> numbers;

//	vector< int * > ints;
//
//	ints.push_back(new int(1));
//
//	cout << ints.size() << endl;

	HTree<float> t;
	rand();

	printVec(t.encode(1), numbers);
	printVec(t.encode(2), numbers);
	printVec(t.encode(3), numbers);
	printVec(t.encode(4), numbers);
	printVec(t.encode(3), numbers);
	printVec(t.encode(5), numbers);


	std::vector<float> startingValues = t.getFirstValueList();

	HTree<float> a(startingValues);


	vector<float> ans = a.decode(numbers);

	for (vector<float>::iterator it = ans.begin(); it != ans.end(); ++it)
		cout << (*it) << endl;

	return 0;
}
