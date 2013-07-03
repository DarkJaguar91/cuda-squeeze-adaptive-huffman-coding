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
#include <fstream>
#include <time.h>
#include <string>
#include <sstream>

#include "AdaptiveHuffmanCoder/Tree.h"
#include "Timer/Timer.h"

void flushBuffer(std::vector<byte> & buffer, FILE * fp) {
	unsigned char b = 0;

	for (int z = 0; z < ceil(int(buffer.size()) / 8); ++z) {
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

void createInputFile(long numVals) {
	using namespace std;
	FILE * fp;

	fp = fopen("values.input", "w");

	srand(time(NULL));
	for (long i = 1; i <= numVals; ++i) {
		float l = rand() % 100;
		fwrite(&l, sizeof(float), 1, fp);
	}
	fclose(fp);

	cout << "done creating file" << endl;
}

void encodeFile() {
	using namespace std;

	Timer::tic();

	HTree<float> encoder;

	FILE * readFile;
	FILE * writeFile;

	readFile = fopen("values.input", "r");
	writeFile = fopen("out.ahc", "w");

	if (readFile == NULL) {
		cout << "File not found" << endl;
		return;
	}

	fseek(readFile, 0L, SEEK_END);
	size_t fileSize = ftell(readFile); // get size of the file
	fseek(readFile, 0L, SEEK_SET);

	int numValues = fileSize / sizeof(float);

	vector<byte> bytes;

	for (long i = 0; i < numValues; ++i) {
		float val;

		fread(&val, sizeof(float), 1, readFile);

//		vector<byte> code = encoder.encode(val);
//
//		copy(code.begin(), code.end(), back_inserter(bytes));
//
//		if (bytes.size() >= 512) {
//			vector<byte> writer;
//
//			copy(bytes.begin(), bytes.begin() + 512, back_inserter(writer));
//
//			flushBuffer(writer, writeFile);
//
//			bytes.erase(bytes.begin(), bytes.begin() + 512);
//		}
		if (i % long(0.1f * numValues) == 0)
			cout << "encoded " << i << " values" << endl;
	}
//
//	if (bytes.size() > 0) { // if we havent finished writing
//		for (long i = 0; i < long(bytes.size()) % 8; ++i)
//			bytes.push_back(0);
//
//		flushBuffer(bytes, writeFile);
//	}
//
//	vector<float> uniqueVals = encoder.getUniqueValueOrderedList();
//
//	long numUniqueVals = uniqueVals.size();
//	long numValsEncoded = encoder.getNumberValues();
//	fwrite(&numValsEncoded, sizeof(long), 1, writeFile); // number unique vals
//	fwrite(&uniqueVals[0], sizeof(float), numUniqueVals, writeFile); // the unique vals
//	fwrite(&numUniqueVals, sizeof(long), 1, writeFile);
//	fclose(writeFile);
//	fclose(readFile);
//
	cout << "Encoded in: " << Timer::toc() << endl;
//	cout << "number values : " << numValsEncoded << endl;
//	cout << "number unique values : " << numUniqueVals << endl;
}

void writeVals(std::vector<float> & vals, FILE* fp){
		fwrite(&vals[0], sizeof(float), vals.size(), fp);
}

void decodeFile() {
	using namespace std;

	Timer::tic();

	FILE * input;
	FILE * output;

	input = fopen("out.ahc", "r");
	output = fopen("values.output", "w");

	fseek(input, 0L, SEEK_END); // seek to end of file
	size_t fileSize = ftell(input); // get size of the file
	fseek(input, -sizeof(long), SEEK_END); // seek to one int before end of file

	vector<float> uniqueVals;
	long numUniqueVals;
	long numValsEncoded;

	fread(&numUniqueVals, sizeof(long), 1, input); // read the numuniquevals
	fseek(input, -(2 * sizeof(long) + numUniqueVals * sizeof(float)), SEEK_END); // seek back to before the numVals Encoded
	fread(&numValsEncoded, sizeof(long), 1, input); // read the encoded vals number
	uniqueVals.resize((size_t) numUniqueVals); // set size of the array
	fread(&uniqueVals[0], sizeof(float), numUniqueVals, input); // read the values

	long numChars = fileSize - 2 * sizeof(long) - numUniqueVals * sizeof(float);

	cout << "num chars in file: " << numChars << endl;

	fseek(input, 0L, SEEK_SET); // go back to beginning of file

	HTree<float> decoder(uniqueVals); // initialise decoder tree

	vector<byte> bytes;

	for (long i = 0; i < numChars; ++i) {
		char b;

		fread(&b, sizeof(unsigned char), 1, input);

		for (long i = 0; i < 8; ++i) {
			bytes.push_back(((b >> 7 - i) & 1));
		}

		if (bytes.size() >= 512){
			vector<float> tmp = decoder.decode(bytes, long(floor(512 / numUniqueVals)));
			numValsEncoded -= long(floor(512 / numUniqueVals));

			writeVals(tmp, output);
		}
	}

	vector<float> tmp = decoder.decode(bytes, numValsEncoded);
	writeVals(tmp, output);

//	for (vector<float>::iterator it = ans.begin();
//				it != ans.end(); ++it)
//			cout << *it << " ";

	fclose(input);
	fclose(output);

	cout << "Decoded in: " << Timer::toc() << endl;
}

int main() {
	using namespace std;

//	createInputFile(737418240);

	encodeFile();

//	decodeFile();

	return 0;
}
