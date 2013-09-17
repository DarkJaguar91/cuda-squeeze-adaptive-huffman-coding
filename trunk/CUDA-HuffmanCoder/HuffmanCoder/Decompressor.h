/*
 * Decompressor.h
 *
 *  Created on: 27 Aug 2013
 *      Author: bjtal
 */

#ifndef DECOMPRESSOR_H_
#define DECOMPRESSOR_H_

#include "HTree.h"

class Decompressor: public HTree {
public:
	Decompressor(frequencyValues & map) :
			HTree(map), root(NULL) {
	}
	;

	virtual ~Decompressor() {
		delete (root);
	}
	;

	void initialize() {
		std::vector<Node *> leaves;
		root = createTree(leaves);
	}

	void decode(const HuffCode & code,
			std::vector<float> & floats) {
		Node * pnt = root;
		for (HuffCode::const_iterator it = code.begin(); it != code.end();
				++it) {
			if (*it == false)
				pnt = pnt->left;
			else
				pnt = pnt->right;

			if (pnt->data != NULL) {
				floats.push_back(*pnt->data);
				pnt = root;
			}
		}
	}
private:
	Node * root;
};

#endif /* DECOMPRESSOR_H_ */
