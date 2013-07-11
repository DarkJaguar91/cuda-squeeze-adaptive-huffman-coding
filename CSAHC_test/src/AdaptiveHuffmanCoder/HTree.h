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
	return a->depth < b->depth;
}

class HTree {
public:
	HTree() :
			root(NULL), nullPar(NULL) {
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

	std::vector<byte> & encodeValue(float data) {
		using namespace std;

		if (root == NULL) { // root null
			root = new Node();
			root->right = new Node(new float(data), root, 1, 1, root->code, 1);

//			checkAndAdd(root->right);
			uniqueList[data] = root->right;

			nullPar = root;

			update(root);

//			vector<byte> code;
//			code.push_back(1);

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

//			checkAndAdd(nullPar->left->right);
			uniqueList[data] = nullPar->left->right;

			nullPar = nullPar->left;
			update(nullPar);

			return code;
		}
	}

private:
	Node * root;
	Node * nullPar;
	std::map<float, Node *> uniqueList;

//	std::map<int, std::vector<Node *> > blocks;

	void update(Node * node) {
		using namespace std;

		if (node->parent != NULL) {
			Node * mostSig = getMostSig(node->weight);

			if (mostSig != NULL)
				if (mostSig->parent != NULL)
					if (node != mostSig) {
						if (node->parent != mostSig) {
							exchangeNodes(mostSig, node);
						}
					}
		}

		++node->weight;

//		checkAndAdd(node);

		if (node->parent != NULL)
			update(node->parent);
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

	Node * getMostSig(int weight) {
//		if (weight == 0)
//			return NULL;
//		std::sort(blocks[weight].begin(), blocks[weight].end(), HigherNode);
//
//		return blocks[weight].at(0);

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

//	void checkAndAdd(Node * node) {
//		if (node->parent == NULL)
//			return;
//
//		if (blocks.count(node->weight - 1) > 0) {
//			// remove
//			for (std::vector<Node *>::iterator it =
//					blocks[node->weight - 1].begin();
//					it != blocks[node->weight - 1].end(); ++it) {
//				if ((*it) == node) {
//					blocks[node->weight - 1].erase(it);
//					break;
//				}
//			}
//		}
//		if (blocks.count(node->weight)) {
//			std::vector<Node *> nodes;
//			nodes.push_back(node);
//			blocks[node->weight] = nodes;
//		} else {
//			blocks[node->weight].push_back(node);
//		}
//	}

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
