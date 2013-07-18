//============================================================================
// Name        : CSHC.cpp
// Author      : Brandon James Talbot
// Version     : 0.1 alpha
// Copyright   : BJTAL 2013
// Description : Runner method for Huffman tree
//============================================================================

#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <string>
#include <time.h>
#include <cstdio>
#include <math.h>
#include <omp.h>

#include "HTree.h"
#include "Timer/Timer.h"

using namespace std;

typedef signed long long longVal;

// Create File method
void createNewFloatFile(const char * fileName, const longVal numFloats,
		const int numUnique) {
	cout << "Creating File ...";
	std::flush(cout);
	FILE* out;

	out = fopen(fileName, "w");

	srand(time(NULL));

	longVal value = numFloats;
	fwrite(&value, sizeof(longVal), 1, out);

	for (longVal i = 0; i < numFloats; ++i) {
		float val = rand() % numUnique;
		fwrite(&val, sizeof(float), 1, out);
	}

	fclose(out);
	cout << "done" << endl;
}
///////////////////////////////////////////////////////////////////////////////////////

void flushBuffer(std::vector<bit> & buffer, FILE * fp) {
	unsigned char b = 0;

	for (int z = 0; z < floor(int(buffer.size()) / 8); ++z) {
		for (int i = 0; i < 8; ++i) {
			if (buffer[i + 8 * z] == 1)
				b |= (1 << (7 - (i % 8)));
			else
				b &= ~(1 << (7 - (i % 8)));
		}

		fwrite(&b, sizeof(unsigned char), 1, fp);
		b = 0;
	}
}

void getDataFromFile(FILE * fp, int numValues, longVal fileSize, longVal currentPos, vector<float> & array) {
	long numToRead = long(min(longVal(numValues), fileSize - currentPos));

	array.resize(numToRead);
	fread(&array[0], sizeof(float), numToRead, fp);
}

void compressFile(const char * inputFileName, const char * outputFileName) {
	cout << "Compressing ...";
	std::flush(cout);

	FILE * input = fopen(inputFileName, "r");
	FILE * output = fopen(outputFileName, "w");

	longVal currentPos = 0;
	longVal fileSize;
	fread(&fileSize, sizeof(longVal), 1, input);

	double totTime = 0;
	longVal numberToRead = 1024 * 1024 * 100 / sizeof(float);
	while (currentPos < fileSize){
		vector<float> numbers;
		vector<bit> code;


		getDataFromFile(input, numberToRead, fileSize, currentPos, numbers);
		currentPos += numberToRead;

		Timer::tic();
		HTree tree(numbers);
		totTime += Timer::toc();

		vector<uniqueValue> unique = tree.getUniqueList();
		long numUnique = unique.size();
		fwrite(&numUnique, sizeof(long), 1, output);
		fwrite(&unique[0], sizeof(uniqueValue), numUnique, output);

		Timer::tic();
		for (long i = 0; i < int(numbers.size()); ++i){
			tree.getCode(numbers[i], code);

			if (code.size() > 1024 * 1024 * 1024){
				cout << "Had to write out the code early\n";
				flushBuffer(code, output);
				code.erase(code.begin(), code.begin() + (floor(code.size() / 8) * 8));
			}
		}

		totTime += Timer::toc();

		for (int i = 0; i < int(code.size() % 8); ++i)
			code.push_back(0);

		flushBuffer(code, output);
		char endll = '\n';
		fwrite(&endll, sizeof(char), 1, output);
	}

	fclose(input);
	fclose(output);

	cout << "Done in: " << totTime << "\n";
}

int main() {
	longVal size = 1024 * 1024 * 1024;
	size /= sizeof(float);
	createNewFloatFile("values.input", size, 9000);

	compressFile("values.input", "out.hc");

	return 0;
}