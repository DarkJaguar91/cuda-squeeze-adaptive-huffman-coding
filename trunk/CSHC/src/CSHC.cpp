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

void getDataFromFile(FILE * fp, int numValues, longVal fileSize,
		longVal currentPos, vector<float> & array) {
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

	do {
		vector<float> numbers;
		getDataFromFile(input, numberToRead, fileSize, currentPos, numbers);
		currentPos += numberToRead;

		vector<bit> code;

		Timer::tic();
		HTree tree(numbers);
		totTime += Timer::toc();

		vector<uniqueValue> unique = tree.getUniqueList();
		long numUnique = unique.size();
		fwrite(&numUnique, sizeof(long), 1, output);
		fwrite(&unique[0], sizeof(uniqueValue), numUnique, output);

		Timer::tic();
		for (long i = 0; i < int(numbers.size()); ++i) {
			tree.getCode(numbers[i], code);

//			if (code.size() > 1024 * 1024 * 1024) {
//				cout << "Had to write out the code early\n";
//				flushBuffer(code, output);
//				code.erase(code.begin(),
//						code.begin() + (floor(code.size() / 8) * 8));
//			}
		}

		totTime += Timer::toc();

		for (int i = 0; i < int(code.size() % 8); ++i)
			code.push_back(0);

		long numchars = code.size() / 8;
		fwrite(&numchars, sizeof(long), 1, output);
		flushBuffer(code, output);
		unsigned char endll = '\n';
		fwrite(&endll, sizeof(unsigned char), 1, output);
		bit cont = currentPos < fileSize ? 0 : 1;
		fwrite(&cont, sizeof(bit), 1, output);
	} while (currentPos < fileSize);

	fclose(input);
	fclose(output);

	cout << "Done in: " << totTime << "\n";
}

void decompressFile(const char * inputFileName, const char * outputFileName) {
	cout << "De-compressing ...";
	std::flush(cout);
	FILE * input = fopen(inputFileName, "r");
	FILE * output = fopen(outputFileName, "w+");

	bool first = true;
	double tottime = 0;

//	while (fgetc(input) != EOF) {
	readAgain: {
		std::flush(cout);
		long numUnique;
		vector<uniqueValue> unique;
		vector<float> vals;
		vector<bit> code;
		long numChars = 0;

		fread(&numUnique, sizeof(long), 1, input);
		unique.resize(numUnique);
		fread(&unique[0], sizeof(uniqueValue), numUnique, input);
		fread(&numChars, sizeof(long), 1, input);

		Timer::tic();
		HTree tree(unique);

		unsigned char c;
		fread(&c, sizeof(unsigned char), 1, input);
		for (int z = 0; z < numChars; ++z) {
			for (long i = 0; i < 8; ++i) {
				code.push_back(((c >> (7 - i)) & 1));
			}

			fread(&c, sizeof(unsigned char), 1, input);
		};

		std::flush(std::cout);
		tree.getValue2(code, vals);

		tottime += Timer::toc();

		longVal numVals = 0;
		if (first)
			first = false;
		else {
			fseek(output, 0L, SEEK_SET);
			fread(&numVals, sizeof(longVal), 1, output);
		}
		fseek(output, 0L, SEEK_SET);
		numVals += vals.size();
		fwrite(&numVals, sizeof(longVal), 1, output);
		fseek(output, 0L, SEEK_END);
		fwrite(&vals[0], sizeof(float), vals.size(), output);
	}
	bit testCont;

	fread(&testCont, sizeof(bit), 1, input);

	if (testCont == 0)
		goto readAgain;
//	}

	fclose(input);
	fclose(output);

	cout << " Done in: " << tottime << endl;
}

void compareFiles(const char * file1, const char * file2) {
	FILE * f1, *f2;

	f1 = fopen(file1, "r");
	f2 = fopen(file2, "r");

	if (f1 == NULL || f2 == NULL) {
		cout << "a file name is incorrect..." << endl;
		return;
	}

	fseek(f1, 0L, SEEK_END);
	size_t f1Size = ftell(f1);
	fseek(f1, 0L, SEEK_SET);

	fseek(f2, 0L, SEEK_END);
	if (long(f1Size) != long(ftell(f2))) {
		cout << "sizes are different" << endl;
		fclose(f1);
		fclose(f2);
		return;
	}
	fseek(f2, 0L, SEEK_SET);

	longVal v1, v2;
	fread(&v1, sizeof(longVal), 1, f1);
	fread(&v2, sizeof(longVal), 1, f2);

	if (v1 != v2) {
		cout << "Different number of elements!" << endl;
		fclose(f1);
		fclose(f2);
		return;
	}

	for (int i = 0; i < v1; ++i) {
		float val1, val2;
		fread(&val1, sizeof(float), 1, f1);
		fread(&val2, sizeof(float), 1, f2);

		if (val1 != val2) {
			cout << "Files not the Same!" << endl;
			fclose(f1);
			fclose(f2);
			return;
		}
	}

	fclose(f1);
	fclose(f2);

	cout << "Files Matched" << endl;
}

int main() {
//	longVal size = 1024 * 1024 * 1;
//	size /= sizeof(float);
//	createNewFloatFile("values.input", size, (50));
//
//	compressFile("values.input", "out.hc");
//
//	decompressFile("out.hc", "values.output");
//
//	compareFiles("values.input", "values.output");

	vector<float> floats;
	srand(time(NULL));
	for (int i = 0; i < ((1024 * 1024) / sizeof(float)); ++i) {
		floats.push_back(rand() % 50);
	}


	cout <<"hello" << endl;
	Timer::tic();
	HTree f(floats);
	flush(cout);

	vector<bit> code;

	for (int i = 0; i < ((1024 * 1024) / sizeof(float)); ++i) {
		f.getCode(floats[i], code);
	}
	cout << "took: " << Timer::toc() << endl;

	return 0;
}
