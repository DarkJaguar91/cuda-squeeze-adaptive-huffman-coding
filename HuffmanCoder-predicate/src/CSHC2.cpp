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

	FILE *input, *output;

	input = fopen(inputFileName, "r");
	output = fopen(outputFileName, "w");

	longValue numberOfFloatsPerBlock = 1024 * 1024 * 1024 / sizeof(float);
	longValue numberOfFloats;
	fread(&numberOfFloats, sizeof(longValue), 1, input);

	longValue numberOfBlocks = ceil(
			numberOfFloats / float(numberOfFloatsPerBlock));
	fwrite(&numberOfBlocks, sizeof(longValue), 1, output);

	for (longValue block = 0; block < numberOfBlocks; ++block) {
		vector<float> numbers;
		frequencyMap map;
		vector<HuffCode *> codes;

		// read values
		longValue numberToRead = min(numberOfFloatsPerBlock,
				numberOfFloats - (block * numberOfFloatsPerBlock));
		readDataFromFile(input, numbers, numberToRead);

		HuffmanTree tree(numbers, codes, map);

		// write out the unique numbers
		longValue numberOfUniqueValues = map.size();
		fwrite(&numberOfUniqueValues, sizeof(longValue), 1, output);
		for (frequencyMap::const_iterator it = map.begin(); it != map.end();
				++it) {
			float value = it->first;
			longValue count = *it->second;

			fwrite(&value, sizeof(float), 1, output);
			fwrite(&count, sizeof(longValue), 1, output);
		}

		fwrite(&numberToRead, sizeof(longValue), 1, output);
		HuffCode code, predicate;
		for (vector<HuffCode *>::const_iterator it = codes.begin();
				it != codes.end(); ++it) {
			std::copy((*it)[0].begin(), (*it)[0].end(),
					std::back_inserter(predicate));
			std::copy((*it)[1].begin(), (*it)[1].end(),
					std::back_inserter(code));
		}
		for (int i = 0; i < int(predicate.size() % 8); ++i)
			predicate.push_back(false);
		for (int i = 0; i < int(code.size() % 8); ++i)
			code.push_back(false);

		flushBuffer(predicate, output);
		flushBuffer(code, output);
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

void getCode(const longValue & numChars, HuffCode & code, FILE * input) {
	for (int i = 0; i < numChars; ++i) {
		unsigned char c;
		fread(&c, sizeof(unsigned char), 1, input);
		for (long i = 0; i < 8; ++i) {
			code.push_back(((c >> (7 - i)) & 1));
		}
	}
}

void decompress(const char * inputFile, const char * outputFile) {
	using namespace std;
	cout << "Decompressing ...";
	flush(cout);
	FILE * input, *output;

	input = fopen(inputFile, "r");
	output = fopen(outputFile, "w");

	longValue numBlocks;
	fread(&numBlocks, sizeof(longValue), 1, input);

	vector<float> floats;
	for (longValue i = 0; i < numBlocks; ++i) {
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

		longValue numberOfValues;
		fread(&numberOfValues, sizeof(longValue), 1, input);

		HuffCode predicates;
		vector<HuffCode> codes;
		getCode(longValue(ceil((numberOfValues * 2) / float(8))), predicates, input);

		for (int z = 0; z < numberOfValues; ++z) {
			HuffCode c;
			bool c1 = predicates[z * 2];
			bool c2 = predicates[(z * 2) + 1];

			if (c1 == false && c2 == false){
				getCode(1, c, input);
			}
			else if (c1 == false && c2 == true){
				getCode(2, c, input);
			}
			else if (c1 == true && c2 == false){
				getCode(3, c, input);
			}
			else {
				getCode(4, c, input);
			}
			codes.push_back(c);
		}

		HuffmanTree tree(map, codes, floats);
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

	longValue numberFloats = 1024 * 1024 * 100;
	numberFloats /= sizeof(float);
	createRandomFile("input", numberFloats, (9000 / 17) / 10);

	Timer::tic();
	compress("input", "output");
	std::cout << Timer::toc() << std::endl;
	Timer::tic();
	decompress("output", "outFloats");
	std::cout << Timer::toc() << std::endl;

	compareFiles("input", "outFloats");

	return 0;
}
