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

#include <omp.h>

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
		float l = rand() % 255;
		fwrite(&l, sizeof(float), 1, fp);
	}
	fclose(fp);

	cout << "done creating file" << endl;
}

void encodeFileParallel() {
	using namespace std;
	Timer::tic();

	int numEncoders = omp_get_max_threads();

	// create the encoders
	HTree<float> * encoders = new HTree<float> [numEncoders];
	FILE ** wfs = new FILE*[numEncoders];

	// read file
	FILE ** rfs = new FILE*[numEncoders];

	// open the files
	for (int i = 0; i < numEncoders; ++i) {
		stringstream s;
		s << "out" << i << ".ahc";
		wfs[i] = fopen(s.str().c_str(), "w");
		rfs[i] = fopen("values.input", "r");
	}

	// get number of bits in file
	fseek(rfs[0], 0L, SEEK_END);
	size_t fileSize = ftell(rfs[0]); // get size of the file
	fseek(rfs[0], 0L, SEEK_SET);

	// calc num floats in file
	int numValues = fileSize / sizeof(float);

	vector<byte> * bytes = new vector<byte> [numEncoders];

	int numValuesPerFile = ceil(numValues / numEncoders);

	for (int i = 0; i < numEncoders; ++i)
		fseek(rfs[i], (i * numValuesPerFile + 1) * sizeof(float), SEEK_SET);

#pragma omp parallel for
	for (int i = 0; i < numEncoders; ++i) {
		for (int z = 0; z < numValuesPerFile; ++z) {
			float val;

			fread(&val, sizeof(float), 1, rfs[i]);

			if (val != NULL) {
				vector<byte> code = encoders[i].encode(val);

				copy(code.begin(), code.end(), back_inserter(bytes[i]));

				if (bytes[i].size() >= 512) {
					vector<byte> writer;

					copy(bytes[i].begin(), bytes[i].begin() + 512,
							back_inserter(writer));

					flushBuffer(writer, wfs[i]);

					bytes[i].erase(bytes[i].begin(), bytes[i].begin() + 512);
				}
			}
		}
	}

#pragma omp parallel for
	for (int i = 0; i < numEncoders; ++i) {
		if (bytes[i].size() > 0) { // if we havent finished writing
			for (long z = 0; z < long(bytes[i].size()) % 8; ++z)
				bytes[i].push_back(0);

			flushBuffer(bytes[i], wfs[i]);
		}
	}

	//close the files
	for (int i = 0; i < numEncoders; ++i) {
		vector<float> uniqueVals = encoders[i].getUniqueValueOrderedList();

		long numUniqueVals = uniqueVals.size();
		long numValsEncoded = encoders[i].getNumberValues();
		fwrite(&numValsEncoded, sizeof(long), 1, wfs[i]); // number unique vals
		fwrite(&uniqueVals[0], sizeof(float), numUniqueVals, wfs[i]); // the unique vals
		fwrite(&numUniqueVals, sizeof(long), 1, wfs[i]);
		fclose(wfs[i]);
		fclose(rfs[i]);
	}

	cout << "Encoded in: " << Timer::toc() << endl;
//	delete(encoders);
//	delete(fps);
//	delete(bytes);
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

	int bufferSize = 512;

	for (long i = 0; i < numValues; ++i) {
		float val;

		fread(&val, sizeof(float), 1, readFile);

		vector<byte> code = encoder.encode(val);

		copy(code.begin(), code.end(), back_inserter(bytes));

		if (int(bytes.size()) >= bufferSize) {
			vector<byte> writer;

			copy(bytes.begin(), bytes.begin() + bufferSize, back_inserter(writer));

			flushBuffer(writer, writeFile);

			bytes.erase(bytes.begin(), bytes.begin() + bufferSize);
		}
	}

	if (bytes.size() > 0) { // if we havent finished writing
		for (long i = 0; i < long(bytes.size()) % 8; ++i)
			bytes.push_back(0);

		flushBuffer(bytes, writeFile);
	}

	vector<float> uniqueVals = encoder.getUniqueValueOrderedList();

	long numUniqueVals = uniqueVals.size();
	long numValsEncoded = encoder.getNumberValues();
	fwrite(&numValsEncoded, sizeof(long), 1, writeFile); // number unique vals
	fwrite(&uniqueVals[0], sizeof(float), numUniqueVals, writeFile); // the unique vals
	fwrite(&numUniqueVals, sizeof(long), 1, writeFile);
	fclose(writeFile);
	fclose(readFile);

	cout << "Encoded in: " << Timer::toc() << endl;
	cout << "number values : " << numValsEncoded << endl;
	cout << "number unique values : " << numUniqueVals << endl;
}

void writeVals(std::vector<float> & vals, FILE* fp) {
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

		if (bytes.size() >= 512) {
			vector<float> tmp = decoder.decode(bytes,
					long(floor(512 / numUniqueVals)));
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

//	createInputFile(268435456);
//	createInputFile((1024*1024*1024) / 4);
	createInputFile(1024 * 1024);
	cout << endl;

	cout << "Encoding:\n";
	std::flush(cout);
	encodeFile();
	cout << endl;

	cout << "Encoding Parallel - with " << omp_get_max_threads()
			<< " threads \n";
	std::flush(cout);
	encodeFileParallel();
	cout << endl;

	cout << "Decoding:\n";
	std::flush(cout);
	decodeFile();

	return 0;
}
