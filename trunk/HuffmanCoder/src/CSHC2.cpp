#include <iostream>
#include <algorithm>
#include <math.h>
#include <time.h>
#include <cstdio>

#include "HuffmanCoder/HuffmanTree.h"
#include "Timer/Timer.h"

void createRandomFile(const char * filename, longValue numberFloats,
		int numberUnique) {
	using namespace std;
	cout << "Creating file ...";
	flush(cout);

	FILE * output = fopen(filename, "w");

	srand(time(NULL));

	fwrite(&numberFloats, sizeof(longValue), 1, output);

	for (longValue i = 0; i < numberFloats; ++i) {
		float val = rand() % numberUnique;
		fwrite(&val, sizeof(float), 1, output);
	}

	fclose(output);
	cout << "Done\n";
}

void readDataFromFile(FILE * input, std::vector<float>& numbers,
		const longValue & numberToRead) {
	numbers.resize(numberToRead);

	fread(&numbers[0], sizeof(float), numberToRead, input);
}

void flushBuffer(const HuffCode & buffer, FILE * fp) {
	unsigned char b = 0;

	for (longValue z = 0; z < floor(longValue(buffer.size()) / 8); ++z) {
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

void compress(const char * inputFileName, const char * outputFileName) {
	using namespace std;
	cout << "Compressing ...";
	flush(cout);

	longValue numberPerBlock = 1024 * 1024 * 1024 / sizeof(float);

	FILE * input, *output;

	input = fopen(inputFileName, "r");
	output = fopen(outputFileName, "w");

	longValue numberFloats;
	fread(&numberFloats, sizeof(longValue), 1, input);

	int numBlocks = ceil(numberFloats / float(numberPerBlock));

	fwrite(&numBlocks, sizeof(int), 1, output);

	for (longValue i = 0; i < ceil(numberFloats / float(numberPerBlock)); ++i) {
		std::vector<float> numbers;
		frequencyMap map;

		readDataFromFile(input, numbers,
				min(numberPerBlock, numberFloats - (i * numberPerBlock)));

		vector<HuffCode*> code;

//		Timer::tic();
		HuffmanTree tree(numbers, code, map);
//		cout << Timer::toc() << " ";
		/// Write the table header
		longValue numUnique = map.size();
		fwrite(&numUnique, sizeof(longValue), 1, output);
		for (frequencyMap::const_iterator it = map.begin(); it != map.end();
				++it) {
			const float val = it->first;
			const longValue cnt = *it->second;
			fwrite(&val, sizeof(float), 1, output);
			fwrite(&cnt, sizeof(longValue), 1, output);
		}
		///////////////////////////////////////////////

		// write the values to disk
		HuffCode tmpCode;
		for (vector<HuffCode*>::const_iterator it = code.begin();
				it != code.end(); ++it) {
			std::copy((*it)->begin(), (*it)->end(), back_inserter(tmpCode));
		}
		for (int x = 0; x < int(tmpCode.size() % 8); ++x) {
			tmpCode.push_back(false);
		}
		longValue length = tmpCode.size() / 8;
		fwrite(&length, sizeof(longValue), 1, output);
		flushBuffer(tmpCode, output);
		tmpCode.clear();
//		for (vector<HuffCode*>::const_iterator it = code.begin();
//						it != code.end(); ++it) {
//			delete(*it);
//		}
		////////////////////////////////////
	}

	fclose(input);
	fclose(output);
	cout << "Done\n";
}

void loadBits(FILE * input, HuffCode & code) {
	unsigned char c;
	fread(&c, sizeof(unsigned char), 1, input);
	for (long i = 0; i < 8; ++i) {
		code.push_back(((c >> (7 - i)) & 1));
	}
}

void decompress(const char * inputFile, const char * outputFile) {
	using namespace std;
	cout << "Decompressing ...";
	flush(cout);
	FILE * input, *output;

	input = fopen(inputFile, "r");
	output = fopen(outputFile, "w");

	int numBlocks;
	fread(&numBlocks, sizeof(int), 1, input);

	vector<float> floats;
	for (int i = 0; i < numBlocks; ++i) {
		frequencyMap map;

		longValue numberUnique;
		fread(&numberUnique, sizeof(longValue), 1, input);

		for (int z = 0; z < int(numberUnique); ++z) {
			float val;
			longValue cnt;
			fread(&val, sizeof(float), 1, input);
			fread(&cnt, sizeof(longValue), 1, input);
			map[val] = new longValue(cnt);
		}

		longValue numberBytes;
		fread(&numberBytes, sizeof(longValue), 1, input);

		HuffCode code;

		for (int z = 0; z < numberBytes; ++z) {
			unsigned char c;
			fread(&c, sizeof(unsigned char), 1, input);
			for (long i = 0; i < 8; ++i) {
				code.push_back(((c >> (7 - i)) & 1));
			}
		};

//		Timer::tic();
		HuffmanTree tree(map, code, floats);
//		cout << Timer::toc() << " ";
	}

	longValue numFloats = floats.size();
	fwrite(&numFloats, sizeof(longValue), 1, output);
	fwrite(&floats[0], sizeof(float), numFloats, output);

	fclose(input);
	fclose(output);
	cout << "Done\n";
}

void compareFiles(const char * file1, const char * file2) {
	using namespace std;
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

	longValue v1, v2;
	fread(&v1, sizeof(longValue), 1, f1);
	fread(&v2, sizeof(longValue), 1, f2);

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
	using namespace std;

	longValue numberFloats = 1024 * 1024 * 500;
	numberFloats /= sizeof(float);
	createRandomFile("input", numberFloats, (9000 / 17));

	Timer::tic();
	compress("input", "output");
	std::cout << Timer::toc() << std::endl;

	Timer::tic();
	decompress("output", "outFloats");
	std::cout << Timer::toc() << std::endl;

	compareFiles("input", "outFloats");

	return 0;
}
