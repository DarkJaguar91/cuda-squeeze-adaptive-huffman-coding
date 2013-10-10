/*
 * Compressor.h
 *
 *  Created on: 27 Aug 2013
 *      Author: bjtal
 */

#include <omp.h>

#ifndef COMPRESSOR_H_
#define COMPRESSOR_H_

#include "HTree.h"

typedef std::map<float, HuffCode *> HuffMap;

class Compressor: public HTree {
public:
	Compressor(frequencyValues & values) :
			HTree(values) {
	}
	;

	virtual ~Compressor() {
		for (HuffMap::iterator it = map.begin(); it != map.end(); ++it) {
			if (it->second != NULL)
				delete (it->second);
		}
	}
	;

	void initialize(const float * values, const longValue & numFloats) {
		binData(values, numFloats);

		std::vector<Node *> leaves;

		Node * root = createTree(leaves);

		calcCodes(leaves);

		delete (root);
	}
	
	void initialize() {
		std::vector<Node *> leaves;

		Node * root = createTree(leaves);
		calcCodes(leaves);

		delete (root);
	}

	void compress(const float * values, HuffCode ** code,
			const longValue & numFloats) {
		longValue i;

		for (i = 0; i < numFloats; ++i) {
			code[i] = map[values[i]];
		}
	}

	HuffMap map;
private:
	void binData(const float * values, const longValue & numValues) {
		longValue i;
#pragma omp parallel for private(i) schedule(dynamic)
		for (i = 0; i < numValues; ++i) {
			float val = values[i];

			if (frequencyMap.count(val) <= 0) {
#pragma omp critical
				{
					if (frequencyMap.count(val) <= 0)
						frequencyMap[val] = new longValue(1);
					else
						++(*frequencyMap[val]);
				}
#pragma omp flush
			} else {
#pragma omp atomic
				++(*frequencyMap[val]);
			}
		}
	}

	void calcCodes(const std::vector<Node *> & leaf) {
		using namespace std;

		for (std::vector<Node *>::const_iterator it = leaf.begin();
				it != leaf.end(); ++it) {

			Node * node = *it;
			Node * par = node->parent;

			HuffCode * code = new HuffCode();

			while (par->parent != NULL) {
				if (node == par->left)
					code->push_back(false);
				else
					code->push_back(true);

				node = par;
				par = par->parent;
			}
			if (node == par->left)
				code->push_back(false);
			else
				code->push_back(true);

			std::reverse(code->begin(), code->end());

			map[*(*it)->data] = code;
		}
	}
};

#endif /* COMPRESSOR_H_ */
