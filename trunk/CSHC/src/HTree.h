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
#include <bitset>

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
			data(d), weight(w), code(NULL), parent(par), left(NULL), right(NULL) {
	}

	~Node() {
		if (data != NULL)
			delete (data);
	}

	float * data;
	long weight;

	std::vector<bit> * code;

	Node * parent;
	Node * left;
	Node * right;
};

struct compareNode {
	bool operator ()(const Node * a, const Node * b) {
		if (long(b->weight) == long(a->weight)) {
			if (b->data == NULL)
				return 0;
			else if (a->data == NULL)
				return 1;
		}
		return b->weight < a->weight;
	}
};

struct compareCode {
	bool operator ()(const std::vector<bit> * a, const std::vector<bit> * b) {
		return b->size() < a->size();
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

	HTree(std::vector<uniqueValue> & vals) :
			root(NULL) {
		setUpTree(vals);
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

		setEnvVars();
	}

	void setUpTree(std::vector<uniqueValue> & vals) {
		using namespace std;
		if (root != NULL)
			deleteNode(root);

		std::map<float, Node *> ul;
		for (std::vector<uniqueValue>::iterator it = vals.begin();
				it != vals.end(); ++it) {
			ul[(*it).value] = new Node(new float((*it).value), NULL,
					(*it).count);
		}

		uniqueList = ul;

		createTree();

		calcCodes();

		createStringList();

		setEnvVars();
	}

	void print() {
		if (root != NULL)
			printTree(root, 0);
	}

	std::vector<bit> getCode(float val) {
		if (uniqueList.count(val) > 0)
			return *uniqueList.find(val)->second->code;
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

	void getValue2(std::vector<bit> & code, std::vector<float> & vals) {
		using namespace std;
		int cpos = 0;
		while ((code.size() - cpos) > 8) {
			string front = string(code.begin() + cpos, code.begin() + cpos + 1);
			string *check = NULL;

			if (front == zz) { //code[cpos] == 0 && code[cpos + 1] == 0) {
				check = new string(code.begin() + cpos,
						code.begin() + 10 + cpos);
			} else if (front == zo) { //code[cpos] == 0 && code[cpos + 1] == 1) {
				check = new string(code.begin() + cpos,
						code.begin() + 18 + cpos);
			} else if (front == oz) { //code[cpos] == 1 && code[cpos + 1] == 0) {
				check = new string(code.begin() + cpos,
						code.begin() + 26 + cpos);
			} else {
				check = new string(code.begin() + cpos,
						code.begin() + 34 + cpos);
			}

			//code.erase(code.begin(), code.begin() + check->length());

			cpos += check->length();

			vals.push_back(codeList[*check]);
			delete (check);
		}
	}

	void getValue(std::vector<bit> & code, std::vector<float> & vals) {
		using namespace std;
		Node * tmp = root; // set a temp node to root

		while (code.size() > 0) { // while we still have values to read
			Node * lookAt = NULL; // create a temp child node

			if (code.at(0) == 0) { // move to left child if placer is 0
				lookAt = tmp->left;
			} else if (code.at(0) == 1) { // move to right child if placer is 1
				lookAt = tmp->right;
			} else {
				return;
			}

			if (lookAt == NULL) {
				cout << "HUGE PROBLEM\n";
			} else if (lookAt->data != NULL) { // if we are at a leaf node
				vals.push_back(*lookAt->data); // get the data value
				code.erase(code.begin());
				return;
			}

			tmp = lookAt;

			code.erase(code.begin()); // delete one placer since we have used it
		}
	}

	void getCode(float val, std::vector<bit> & code) {
		using namespace std;

		if (uniqueList.count(val) > 0) {
			vector<bit> tmp = *uniqueList.find(val)->second->code;

			copy(tmp.begin(), tmp.end(), back_inserter(code));
		}
	}

	long getNumValuesEncoded() {
		return root->weight;
	}

private:
	Node * root;
	std::map<float, Node *> uniqueList;
	std::map<std::string, float> codeList;
	std::string zz, zo, oz;

	void setEnvVars() {
		unsigned char z = 0;
		unsigned char o = 1;

		zz = z + z;
		zo = z + o;
		oz = o + z;
	}

	void createStringList() {
//#pragma omp parallel
		{
			for (std::map<float, Node *>::iterator it = uniqueList.begin();
					it != uniqueList.end(); ++it) {
//#pragma omp single nowait
				{
					std::string code(it->second->code->begin(),
							it->second->code->end());

					codeList[code] = *it->second->data;
				}
			}
		}
	}

	void calcCodes() {
		using namespace std;

//#pragma omp parallel
		{
			for (std::map<float, Node *>::iterator it = uniqueList.begin();
					it != uniqueList.end(); ++it) {
//#pragma omp single nowait
				{
					Node * node = it->second;
					Node * par = node->parent;

					vector<bit> * code = new vector<bit>();

					while (par->parent != NULL) {
						if (node == par->left)
							code->push_back(0);
						else
							code->push_back(1);

						node = par;
						par = par->parent;
					}
					if (node == par->left)
						code->push_back(0);
					else
						code->push_back(1);

					if (code->size() > 24) {
						union {
							float input;
							int output;
						} data;

						data.input = *it->second->data;

						std::bitset<32> bits(data.output);

						code = new vector<bit>();
						code->push_back(1);
						code->push_back(1);
						for (int i = 0; i < 32; ++i)
							code->push_back(bits[i]);

					} else {
						for (int i = 0; i < (int) (code->size() % 8); ++i)
							code->push_back(0);

						if (code->size() == 8) {
							code->push_back(0);
							code->push_back(0);
						} else if (code->size() == 16) {
							code->push_back(0);
							code->push_back(1);
						} else {
							code->push_back(1);
							code->push_back(0);
						}

						std::reverse(code->begin(), code->end());
					}

					it->second->code = code;
				}
			}
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
#pragma omp parallel for shared(ul) private(i) schedule(dynamic)
		for (i = 0; i < int(values.size()); ++i) {
			float val = values.at(i);

			if (ul.count(val) <= 0) {
#pragma omp critical
				{
					if (ul.count(val) <= 0)
						ul[val] = new Node(new float(val), NULL, 1);
					else
						ul.find(val)->second->weight++;
				}
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
