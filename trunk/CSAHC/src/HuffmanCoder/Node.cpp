/*
 * Node.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: Brandon
 */

#include "Node.h"

namespace HuffmanCoder {

Node::Node(Node * par) : data(0), count(0), parent(par), one(NULL), zero(NULL) {}

Node::Node(const float dta, Node * par) : data(dta), count(1), parent(par), one(NULL), zero(NULL) {}

Node::~Node() {
	// no need cus no pointers to delete
}

} /* namespace HuffmanCoder */
