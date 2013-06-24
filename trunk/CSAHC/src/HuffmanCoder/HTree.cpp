/*
 * Tree.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: Brandon
 */

#include "HTree.h"

namespace HuffmanCoder {

HTree::HTree() {
	root = NULL;
}

HTree::~HTree() {
	if (root != NULL)
		free(root);
}

// Data Printing Methods ////////////////////////////////////
void HTree::print() {
	printNode(0, root);
}

void HTree::printNode(int depth, Node * node) {
	if (node != NULL) {
		using namespace std;

		for (int i = 0; i < depth; ++i)
			cout << "\t";

		cout << "(" << node->data << " -> " << node->count << ")" << endl;

		printNode(depth + 1, node->zero);
		printNode(depth + 1, node->one);
	}
}
/////////////////////////////////////////////////////////////////

// Data Finding Methods //////////////////////////////////////////////////
bool HTree::checkForData(float value) {
	if (root == NULL)
		return false;
	return (findValue(root, value)) == NULL ? false : true;
}

Node * HTree::findValue(Node * n, float value) {
	if (n == NULL) {
		return NULL;
	} else if (n->data == value) {
		return n;
	} else {
		Node * z = findValue(n->one, value);
		if (z == NULL)
			return findValue(n->zero, value);
		else
			return z;
	}
}

float HTree::findValueNodeData(float value) {
	Node * n = findValue(root, value);

	return n->data;
}
/////////////////////////////////////////////////////////////////////////

// Data Adding methods //////////////////////////////////////////////////
void HTree::addData(float value) {
	if (root == NULL) {
		root = new Node(NULL);
		root->one = new Node(value, root);
	} else {
		Node * n = findValue(root, value);

		if (n == NULL) {
			Node * nullChildNode = findNullChildNode(root);

			nullChildNode->zero = new Node(nullChildNode);
			nullChildNode->zero->one = new Node(value, nullChildNode->zero);
		} else
			++n->count;
	}
}

Node * HTree::findNullChildNode(Node * node) {
	if (node->one == NULL) {
		return NULL;
	} else if (node->zero == NULL) {
		return node;
	} else {
		Node * n = findNullChildNode(node->zero);
		if (n != NULL)
			return n;
		else
			return findNullChildNode(node->one);
	}
}

////////////////////////////////////////////////////////////////////////////

} /* namespace HuffmanCoder */
