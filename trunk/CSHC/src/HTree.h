//============================================================================
// Name        : HTree.h
// Author      : Brandon James Talbot
// Version     : 0.1 alpha
// Copyright   : BJTAL 2013
// Description : H File for tree class of normal Huffman
//============================================================================

#include <iostream>
#include <queue>
#include <map>
#include <vector>
#include <algorithm>
#include <omp.h>

#ifndef HTREE_H_
#define HTREE_H_

typedef unsigned char bit;

struct uniqueValue {
	uniqueValue(float value = 0, long count = 0) :
			value(value), count(count) {
	}

	float value;
	long count;
};

struct Node {
	Node(float * d = NULL, Node * par = NULL, long w = 0) :
			data(d), weight(w), parent(par), left(NULL), right(NULL) {
	}

	~Node() {
		if (data != NULL)
			delete (data);
	}

	float * data;
	long weight;

	std::vector<bit> code;

	Node * parent;
	Node * left;
	Node * right;
};

struct compareNode {
	bool operator ()(const Node * a, const Node * b) {
		if (long(b->weight) == long(a->weight))
			if (b->data == NULL)
				return 0;
			else if (a->data == NULL)
				return 1;
		return b->weight < a->weight;
	}
};

class HTree {
public:
	HTree() :
			root(NULL) {
		using namespace std;
	}

	HTree(std::vector<float> & values) :
			root(NULL) {
		setUpTree(values);
	}

	~HTree() {
		if (root != NULL)
			deleteNode(root);
	}

	void setUpTree(std::vector<float> & values) {
		// memory check
		if (root != NULL)
			deleteNode(root);

		binData(values);

		createTree();

		calcCodes();
	}

	void print() {
		if (root != NULL)
			printTree(root, 0);
	}

	std::vector<bit> getCode(float val) {
		if (uniqueList.count(val) > 0)
			return uniqueList.find(val)->second->code;
		return std::vector<bit>();
	}

	std::vector<uniqueValue> getUniqueList() {
		std::vector<uniqueValue> tmp;
		for (std::map<float, Node *>::iterator it = uniqueList.begin();
				it != uniqueList.end(); ++it) {
			tmp.push_back(uniqueValue(*it->second->data, it->second->weight));
		}
		return tmp;
	}

	void getCode(float val, std::vector<bit> & code) {
		using namespace std;

		if (uniqueList.count(val) > 0) {
			vector<bit> tmp = uniqueList.find(val)->second->code;

			copy(tmp.begin(), tmp.end(), back_inserter(code));
		}
	}

private:
	Node * root;
	std::map<float, Node *> uniqueList;

	void calcCodes() {
		using namespace std;
		for (std::map<float, Node *>::iterator it = uniqueList.begin();
				it != uniqueList.end(); ++it) {
			Node * node = it->second;
			Node * par = node->parent;

			vector<bit> code;

			while (par->parent != NULL) {
				if (node == par->left)
					code.push_back(0);
				else
					code.push_back(1);

				node = par;
				par = par->parent;
			}
			if (node == par->left)
				code.push_back(0);
			else
				code.push_back(1);

			std::reverse(code.begin(), code.end());

			it->second->code = code;
		}
	}

	void createTree() {
		using namespace std;

		priority_queue<Node *, vector<Node *>, compareNode> que;

		for (std::map<float, Node *>::iterator it = uniqueList.begin();
				it != uniqueList.end(); ++it) {
			que.push(it->second);
		}

		while (que.size() > 1) {
			Node * a = que.top();
			que.pop();
			Node * b = que.top();
			que.pop();

			Node * par = new Node(NULL, NULL, a->weight + b->weight);

			par->left = a;
			par->right = b;

			a->parent = par;
			b->parent = par;

			que.push(par);
		}

		root = que.top();
	}

	void binData(std::vector<float> & values) {
		std::map<float, Node *> ul;
		int i;
#pragma omp parallel for schedule(dynamic) private(i) shared(ul)
		for (i = 0; i < int(values.size()); ++i) {
			float val = values.at(i);

			if (ul.count(val) <= 0) {
#pragma omp critical
				ul[val] = new Node(new float(val), NULL, 1);
			} else {
#pragma omp atomic
				ul.find(val)->second->weight++;
			}
		}
		uniqueList = ul;
	}

	void deleteNode(Node * n) {
		if (n != NULL) {
			deleteNode(n->left);
			deleteNode(n->right);

			delete (n);
		}
	}

	void printTree(Node * n, long depth) {
		using namespace std;
		for (int i = 0; i < depth; ++i)
			cout << "\t";

		if (n->data == NULL)
			cout << "NULL";
		else
			cout << *n->data;

		cout << " - " << n->weight << endl;

		if (n->left != NULL)
			printTree(n->left, depth + 1);
		if (n->right != NULL)
			printTree(n->right, depth + 1);
	}
};

#endif
