/*
 * HTree.h
 *
 *  Created on: 08 Jul 2013
 *      Author: root
 */

#ifndef HTREE_H_
#define HTREE_H_

#include <cstdlib>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

typedef unsigned char byte;

class Node {
public:
	Node(float * d = NULL, Node * par = NULL, int w = 0, int dp = 0,
			std::vector<byte> cd = std::vector<byte>(), int insertVal = -1) :
			data(d), weight(w), depth(dp), code(cd), parent(par), left(NULL), right(
					NULL) {
		if (insertVal != -1) {
			code.push_back(insertVal);
		}
	}

	~Node() {
		if (data != NULL)
			delete (data);
	}

	float * data;

	int weight;
	int depth;

	std::vector<byte> code;

	Node * parent;
	Node * left;
	Node * right;
};

bool HigherNode(const Node * a, const Node * b) {
	return a->depth > b->depth;
}

class HTree {
public:
	HTree() :
			root(NULL), nullPar(NULL) {
	}

	HTree(std::vector<float> & newDataValues) :
			root(NULL), nullPar(NULL), tempNewDataHolder(newDataValues) {
	}

	~HTree() {
		if (root != NULL)
			deleteNode(root);
	}

	/**
	 * Prints out the tree
	 */
	void print() {
		printFromNode(root, 0);
	}

	/**
	 * Prints the tree from a specific node
	 */
	void printFromNode(Node * n, long depth) {
		using namespace std;

		for (long i = 0; i < depth; ++i)
			cout << "\t";

		if (n != NULL) {

			if (n->data == NULL)
				cout << "(joiner" << " - " << n->weight << ") ";
			else
				cout << "(" << *(n->data) << " - " << n->weight << ") ";

			vector<byte> code = n->code;

			for (int i = 0; i < code.size(); ++i)
				if (code.at(i) == 0)
					cout << "0";
				else
					cout << "1";

			cout << endl;

			printFromNode(n->left, depth + 1);
			printFromNode(n->right, depth + 1);
		} else {
			cout << "(NULL)\n";
		}
	}

	std::vector<byte> encodeValue(float data) {
		using namespace std;

		if (root == NULL) { // root null
			root = new Node();
			root->right = new Node(new float(data), root, 1, 1, root->code, 1);

			uniqueList[data] = root->right;
			tempNewDataHolder.push_back(data);

			nullPar = root;

			checkNodeBlockPlacement(root->right);

			update(root);

			return root->right->code;
		} // root null if end

		if (uniqueList.count(data) > 0) { // exists
			Node * n = uniqueList.find(data)->second;

			vector<byte> code = n->code;

			update(n);
			return code;
		} else { // dnts exist
			nullPar->left = new Node(NULL, nullPar, 0, nullPar->depth - 2,
					nullPar->code, 0);
			nullPar->left->right = new Node(new float(data), nullPar->left, 1,
					nullPar->left->depth - 1, nullPar->left->code, 1);

			vector<byte> code = nullPar->left->right->code;

			uniqueList[data] = nullPar->left->right;
			tempNewDataHolder.push_back(data);

			nullPar = nullPar->left;

			checkNodeBlockPlacement(nullPar->right);

			update(nullPar);

			return code;
		}
	}

	std::vector<float> decode(std::vector<byte> & values, long numValues) {
		using namespace std;

		Node * tmp = root; // set a temp node to root

		vector<float> output; // the output list of numbers initialiser

		// if root is null
		if (tmp == NULL) {
			root = new Node(); // create the root node
			root->right = new Node(new float(tempNewDataHolder.at(0)), root, 1, 1, root->code, 1);

			checkNodeBlockPlacement(root->right);

			update(root); // update the root node (inc weight)

			output.push_back(tempNewDataHolder.at(0)); // get the value

			tempNewDataHolder.erase(tempNewDataHolder.begin()); // delete already used value

			values.erase(values.begin()); // when adding new value, we have to delete 2 markers
		}

		tmp = root; // reset the tmp value to root

		while (values.size() > 0) { // while we still have values to read
			if (long(output.size()) == numValues)
				return output;
			Node * lookAt = NULL; // create a temp child node

			if (values.at(0) == 0) { // move to left child if placer is 0
				lookAt = tmp->left;
			} else if (values.at(0) == 1) {// move to right child if placer is 1
				lookAt = tmp->right;
			}
			else {
				break;
			}

			if (lookAt == NULL){ // value not found.. add it
				tmp->left = new Node(NULL, tmp, 0, tmp->depth - 2,
						tmp->code, 0);
				tmp->left->right = new Node(new float(tempNewDataHolder.at(0)), tmp->left, 1,
						tmp->left->depth - 1, tmp->left->code, 1);

				checkNodeBlockPlacement(tmp->left->right);

				update(tmp->left); // upate the tree from the parent

				tmp = root; // finished with this value, reset our temp node to root
				output.push_back(tempNewDataHolder.at(0)); // get the value of the node
				tempNewDataHolder.erase(tempNewDataHolder.begin()); // delete used value from our new value list
				values.erase(values.begin()); // when adding a new value we have to delete 2 placers
			}
			else if (lookAt->data != NULL){ // if we are at a leaf node
				output.push_back(*lookAt->data); // get the data value
				update(lookAt); // update from that node as we have reused it
				tmp = root; // found a value, reset our temp node to root
			}
			else { // not yet at a leaf or end node
				tmp = lookAt; // move onward
			}

			values.erase(values.begin()); // delete one placer since we have used it
		}

		return output; // return the numbers decoded
	}

	vector<float> getUniqueVals(){
		return tempNewDataHolder;
	}

	int getNumberValues(){
		return root->weight;
	}

private:
	Node * root;
	Node * nullPar;
	std::map<float, Node *> uniqueList;

	std::map<int, std::vector<Node *> *> blocks;

	std::vector<float> tempNewDataHolder;

	void update(Node * node) {
		using namespace std;

		if (node->parent != NULL) {
			Node * mostSig = getMostSigNew(node->weight); //getMostSig(node->weight);

			if (mostSig != NULL)
				if (node != mostSig) {
					if (node->parent != mostSig) {
						exchangeNodes(mostSig, node);
					}
				}
		}

		++node->weight;

		if (node->parent != NULL) // not root node
			checkNodeBlockPlacement(node);

		if (node->parent != NULL)
			update(node->parent);
	}

	void checkNodeBlockPlacement(Node * n) {
		using namespace std;
		if (blocks.count(n->weight - 1) > 0) { // does already have a vector
			vector<Node *> * list = blocks.find(n->weight - 1)->second;
			if (std::find(list->begin(), list->end(), n) != list->end()) {
				list->erase(remove(list->begin(), list->end(), n), list->end());
			}
		}

		vector<Node *> * list = NULL;

		if (blocks.count(n->weight) > 0) // it exists
			list = blocks.find(n->weight)->second;
		else {
			list = new vector<Node *>();
			blocks[n->weight] = list;
		}

		list->push_back(n);
	}

	void exchangeNodes(Node * n1, Node * n2) {
		using namespace std;
		if (n1->parent->left == n1) { // if left child of parent 1
			if (n2->parent->left == n2) { // if left child of parent 2
				std::swap(n1->parent->left, n2->parent->left); // swap children pointers
			} else {
				std::swap(n1->parent->left, n2->parent->right); // swap for parent 2s right child
			}
		} else { // if right node of parent 1
			if (n2->parent->left == n2) { // if left node of parent 2
				std::swap(n1->parent->right, n2->parent->left); // swap child nodes
			} else {
				std::swap(n1->parent->right, n2->parent->right); // right child of parent 1
			}
		}

		std::swap(n1->code, n2->code);

		checkSubTreeNodes(n1->left, 0);
		checkSubTreeNodes(n1->right, 1);
		checkSubTreeNodes(n2->left, 0);
		checkSubTreeNodes(n2->right, 1);

		std::swap(n1->depth, n2->depth); // swap the parent pointerss
		std::swap(n1->parent, n2->parent); // swap the parent pointers
	}

	void checkSubTreeNodes(Node * n, int val) {
		if (n == NULL)
			return;

		std::vector<byte> cd = n->parent->code;
		cd.push_back(val);
		n->code = cd;

		checkSubTreeNodes(n->right, 1);
		checkSubTreeNodes(n->left, 0);
	}

	Node * getMostSigNew(int weight) {
		using namespace std;

		if (blocks.count(weight) <= 0)
			return NULL;

		vector<Node *> * list = blocks[weight];

		if (list->size() <= 0)
			return NULL;

		std::sort(list->begin(), list->end(), HigherNode);

		return list->at(0);
	}

	Node * getMostSig(int weight) {
		using namespace std;

		Node * n = root;

		vector<Node *> que; // que for breadth tree searching

		que.push_back(n); // insert top node

		while (que.size() > 0) { // while we have no nodes
			n = que.at(0); // get top node
			que.erase(que.begin()); // remove top node

			if (n->weight == weight) // if its the weight we are looking for
				return n; // return it

			if (n->right != NULL) // add right child if needed (check right first always) if not NULL
				que.push_back(n->right);
			if (n->left != NULL) // add left child if needed, if not NULL
				que.push_back(n->left);
		}
		return NULL; // return NULL if not found

	}

	void deleteNode(Node * node) {
		if (node->left != NULL)
			deleteNode(node->left);
		if (node->right != NULL)
			deleteNode(node->right);

		delete (node);
	}

	std::vector<byte> &getCode(Node * node) {
		std::vector<byte> code;

		Node * par = node->parent;

		while (par->parent != NULL) {
			if (par->left == node)
				code.push_back(0);
			else
				code.push_back(1);

			par = par->parent;
			node = node->parent;
		}

		if (par->left == node)
			code.push_back(0);
		else
			code.push_back(1);

		std::reverse(code.begin(), code.end());

		return code;
	}
}
;

#endif /* HTREE_H_ */
