/*
 * Node.h
 *
 *  Created on: Jun 24, 2013
 *      Author: Brandon James Talbot
 */

#ifndef NODE_H_
#define NODE_H_

#include <cstdlib>

namespace HuffmanCoder {

class Node {
public:
	Node(Node * par);
	Node(const float dta, Node * par);
	virtual ~Node();

	friend class HTree;
private:
	float data;
	int count;
	Node * parent;
	Node * one;
	Node * zero;
};

} /* namespace HuffmanCoder */
#endif /* NODE_H_ */
