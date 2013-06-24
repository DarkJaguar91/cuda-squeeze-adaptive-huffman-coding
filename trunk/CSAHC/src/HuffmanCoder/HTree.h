/*
 * Tree.h
 *
 *  Created on: Jun 24, 2013
 *      Author: Brandon
 */

#ifndef HTREE_H_
#define HTREE_H_

#include <iostream>

#include "Node.h"

namespace HuffmanCoder {

class HTree {
public:
	HTree();
	virtual ~HTree();

	void print();
	bool checkForData(float value);
	void addData(float value);

	float findValueNodeData(float value);

private:
	Node * root;

	void printNode(int depth, Node * node);
	Node * findNullChildNode(Node * node);
	Node * findValue(Node * n, float value);
};

} /* namespace HuffmanCoder */
#endif /* HTREE_H_ */
