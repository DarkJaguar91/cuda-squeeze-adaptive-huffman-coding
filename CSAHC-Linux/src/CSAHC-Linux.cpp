//============================================================================
// Name        : CSAHC-Linux.cpp
// Author      : Brandon James Talbot
// Version     :
// Copyright   : BJTalbot 2013
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <math.h>

#include "AdaptiveHuffmanCoder/HTree.h"
#include "AdaptiveHuffmanCoder/HTreeOld.h"
#include "Timer/Timer.h"

using namespace std;

// Create File method
void createNewFloatFile(const char * fileName, const long numFloats,
		const int numUnique) {
	cout << "Creating File ...";
	FILE* out;

	out = fopen(fileName, "w");

	srand(time(NULL));

	for (int i = 0; i < numFloats; ++i) {
		float val = rand() % numUnique;
		fwrite(&val, sizeof(float), 1, out);
	}

	fclose(out);
	cout << "done" << endl << endl;
}
///////////////////////////////////////////////////////////////////////////////////////

// Encoding methods
void flushBits(vector<byte> & code, FILE * out) {
	unsigned char b = 0;

	for (int z = 0; z < ceil(int(code.size()) / 8); ++z) {
		for (int i = 0; i < 8; ++i) {
			if (code[i + 8 * z] == 1)
				b |= (1 << (7 - (i % 8)));
			else
				b &= ~(1 << (7 - (i % 8)));
		}

		fwrite(&b, sizeof(unsigned char), 1, out);
		b = 0;
	}
}

void encodeFile(const char * inputFileName, const char * outputFileName) {
	cout << "Encoding File with new version ";

	Timer::tic();

	HTree encoder;

	FILE * readFile;
	FILE * writeFile;

	readFile = fopen(inputFileName, "r");
	writeFile = fopen(outputFileName, "w");

	if (readFile == NULL) {
		cout << "File not found" << endl;
		return;
	}

	fseek(readFile, 0L, SEEK_END);
	size_t fileSize = ftell(readFile); // get size of the file
	fseek(readFile, 0L, SEEK_SET);

	int numValues = fileSize / sizeof(float);

	vector<byte> bytes;

	int bufferSize = 512;

	for (long i = 0; i < numValues; ++i) {
		float val;

		fread(&val, sizeof(float), 1, readFile);

		vector<byte> code = encoder.encodeValue(val);

		copy(code.begin(), code.end(), back_inserter(bytes));

		if (int(bytes.size()) >= bufferSize) {
			vector<byte> writer;

			copy(bytes.begin(), bytes.begin() + bufferSize,
					back_inserter(writer));

			flushBits(writer, writeFile);

			bytes.erase(bytes.begin(), bytes.begin() + bufferSize);
		}
	}

	if (bytes.size() > 0) { // if we havent finished writing
		for (long i = 0; i < long(bytes.size()) % 8; ++i)
			bytes.push_back(0);

		flushBits(bytes, writeFile);
	}

	vector<float> uniqueVals = encoder.getUniqueVals();

	long numUniqueVals = uniqueVals.size();
	long numValsEncoded = encoder.getNumberValues();
	fwrite(&numValsEncoded, sizeof(long), 1, writeFile); // number unique vals
	fwrite(&uniqueVals[0], sizeof(float), numUniqueVals, writeFile); // the unique vals
	fwrite(&numUniqueVals, sizeof(long), 1, writeFile);
	fclose(writeFile);
	fclose(readFile);

	cout << " : " << Timer::toc() << "seconds" << endl;
}

void encodeFileOLD(const char * inputFileName, const char * outputFileName) {
	cout << "Encoding File with old version ";

	Timer::tic();

	HTreeOld<float> encoder;

	FILE * readFile;
	FILE * writeFile;

	readFile = fopen(inputFileName, "r");
	writeFile = fopen(outputFileName, "w");

	if (readFile == NULL) {
		cout << "File not found" << endl;
		return;
	}

	fseek(readFile, 0L, SEEK_END);
	size_t fileSize = ftell(readFile); // get size of the file
	fseek(readFile, 0L, SEEK_SET);

	int numValues = fileSize / sizeof(float);

	vector<byte> bytes;

	int bufferSize = 512;

	for (long i = 0; i < numValues; ++i) {
		float val;

		fread(&val, sizeof(float), 1, readFile);

		vector<byte> code = encoder.encode(val);

		copy(code.begin(), code.end(), back_inserter(bytes));

		if (int(bytes.size()) >= bufferSize) {
			vector<byte> writer;

			copy(bytes.begin(), bytes.begin() + bufferSize,
					back_inserter(writer));

			flushBits(writer, writeFile);

			bytes.erase(bytes.begin(), bytes.begin() + bufferSize);
		}
	}

	if (bytes.size() > 0) { // if we havent finished writing
		for (long i = 0; i < long(bytes.size()) % 8; ++i)
			bytes.push_back(0);

		flushBits(bytes, writeFile);
	}

	vector<float> uniqueVals = encoder.getUniqueValueOrderedList();

	long numUniqueVals = uniqueVals.size();
	long numValsEncoded = encoder.getNumberValues();
	fwrite(&numValsEncoded, sizeof(long), 1, writeFile); // number unique vals
	fwrite(&uniqueVals[0], sizeof(float), numUniqueVals, writeFile); // the unique vals
	fwrite(&numUniqueVals, sizeof(long), 1, writeFile);
	fclose(writeFile);
	fclose(readFile);

	cout << " : " << Timer::toc() << "seconds" << endl;
}
////////////////////////////////////////////////////////////////////////////////////////////////////

// decode methods
void writeVals(std::vector<float> & vals, FILE* fp) {
	fwrite(&vals[0], sizeof(float), vals.size(), fp);
}

void decodeFile(const char * inputFile, const char * outputfile) {
	cout << "Decoding File with new version ";

	Timer::tic();

	FILE * input;
	FILE * output;

	input = fopen(inputFile, "r");
	output = fopen(outputfile, "w");

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

	fseek(input, 0L, SEEK_SET); // go back to beginning of file

	HTree decoder(uniqueVals); // initialise decoder tree

	vector<byte> bytes;

	int bufferSize = 256;

	for (long i = 0; i < numChars; ++i) {
		char b;

		fread(&b, sizeof(unsigned char), 1, input);

		for (long i = 0; i < 8; ++i) {
			bytes.push_back(((b >> 7 - i) & 1));
		}


		if (bytes.size() >= bufferSize) {
			vector<float> tmp = decoder.decode(bytes,
					long(floor(bufferSize / numUniqueVals)));
			numValsEncoded -= long(floor(bufferSize / numUniqueVals));

			writeVals(tmp, output);
		}
	}

	vector<float> tmp = decoder.decode(bytes, numValsEncoded);
	writeVals(tmp, output);

	fclose(input);
	fclose(output);

	cout << " : " << Timer::toc() << endl;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////

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
	if (f1Size != ftell(f2)){
		cout << "sizes are different" << endl;
		return;
	}
	fseek(f2, 0L, SEEK_SET);

	for (int i = 0; i < int(f1Size / sizeof(float)); ++i){
		float val1, val2;
		fread(&val1, sizeof(float), 1, f1);
		fread(&val2, sizeof(float), 1, f2);

		if (val1 != val2){
			cout << "Files not the Same!" << endl;
			return;
		}
	}

	cout << "Files Matched" << endl;
}

int main() {

	for (int i = 1; i < 11; ++i){
		cout << i << "MB" << endl;
		createNewFloatFile("values.input", (1024 * 1024 * i) / sizeof(float), 255);

		encodeFile("values.input", "out.ahc");

		decodeFile("out.ahc", "values.output");

		compareFiles("values.input", "values.output");
		cout << endl;
	}
	return 0;
}
