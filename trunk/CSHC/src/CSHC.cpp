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

	for (longVal z = 0; z < ceil(longVal(buffer.size()) / 8); ++z) {
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

void getDataFromFile(FILE * fp, longVal numValues, vector<float> & array) {
	longVal currentPos = ftell(fp);
	fseek(fp, 0L, SEEK_END);
	longVal endPos = ftell(fp);
	fseek(fp, currentPos, SEEK_SET);

	longVal readNumber = min(longVal(endPos - currentPos), numValues);

	array.resize(readNumber);
	fread(&array[0], sizeof(float), readNumber, fp);
}

void compressFile(const char * inputFileName, const char * outputFileName) {
	cout << "Compressing ...";
	std::flush(cout);

	FILE * input = fopen(inputFileName, "r");
	FILE * output = fopen(outputFileName, "w");

	fseek(input, 0L, SEEK_END);
	longVal endOfFile = ftell(input);
	fseek(input, 0L, SEEK_SET);

//	cout << endl << endOfFile / 1024 / 1024 / 1024 << endl;

	double totalTime = 0;
	longVal c = 0;
	longVal currentPos = 0;
	longVal size = 1024 * 1024;
	size *= 1024;
	size *= 17;
	size /= sizeof(float);

//	while (longVal(ftell(input)) != longVal(endOfFile)) {
	while (currentPos < size){
		vector<float> numbers;
		vector<bit> code;

		getDataFromFile(input, 1024 * 1024 * 50 / sizeof(float), numbers);
		currentPos += 1024 * 1024 * 50 / sizeof(float);


		Timer::tic();

		HTree tree(numbers);

		for (longVal i = 0; i < longVal(numbers.size()); ++i){
			tree.getCode(numbers[i], code);
		}

		for (longVal i = 0; i < longVal(code.size() % 8); ++i)
			code.push_back(0);

		vector<uniqueValue> uniqueList = tree.getUniqueList();

		totalTime += Timer::toc();

		longVal numUniqueVals = uniqueList.size();
		longVal numBits = code.size() / 8;
		fwrite(&numUniqueVals, sizeof(longVal), 1, output);
		fwrite(&uniqueList[0], sizeof(uniqueValue), numUniqueVals, output);
		fwrite(&numBits, sizeof(longVal), 1, output);
		flushBuffer(code, output);
		++c;
	}

	fclose(input);
	fclose(output);

	cout << "Done in: " << totalTime << "s in " << c << " iterations\n";
}

int main() {

	//	longVal size = 1024 * 1024;
	//	size *= 1024;
	//	size *= 17;
	//	size /= sizeof(float);
//
//	createNewFloatFile("values.input", size, 9000);

	compressFile("values.input", "out.hc");

	return 0;
}
