/*
 * HuffmanTree.h
 *
 *  Created on: 23 Aug 2013
 *      Author: bjtal
 */

#include <iostream>
#include <queue>
#include <map>
#include <iterator>
#include <algorithm>
#include <math.h>
#include <algorithm>
#include <omp.h>
#include <bitset>

#ifndef HUFFMANTREE_H_
#define HUFFMANTREE_H_

typedef unsigned long long longValue;
typedef std::vector<bool> HuffCode;
typedef std::map<float, HuffCode *> HuffMap;
typedef std::map<HuffCode, float *> HuffMapReverse;
typedef std::map<float, longValue*> frequencyMap;

struct Node {
	Node(float * data = NULL, int weight = 0, Node * parent = NULL) :
			data(data), weight(weight), parent(parent), left(NULL), right(NULL) {
	}

	~Node() {
		if (data != NULL)
			delete (data);
		if (left != NULL)
			delete (left);
		if (right != NULL)
			delete (right);
	}

	float * data;
	longValue weight;

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

class HuffmanTree {
public:
	HuffmanTree(const std::vector<float> & values,
			std::vector<HuffCode *> & code, frequencyMap & map) :
			uniqueList(map) {
		setUpTree(values, code);
	}

	HuffmanTree(frequencyMap & map, const std::vector<HuffCode> & code,
			std::vector<float> & numbers) :
			uniqueList(map) {
		setUpTree(numbers, code);
	}

	~HuffmanTree() {
		for (HuffMap::iterator it = map.begin(); it != map.end(); ++it)
			delete[] (it->second);
		for (HuffMapReverse::iterator it = rmap.begin(); it != rmap.end(); ++it)
			delete (it->second);
		for (frequencyMap::iterator it = uniqueList.begin();
				it != uniqueList.end(); ++it)
			delete (it->second);
	}

	frequencyMap & getFrequencyMap() {
		return uniqueList;
	}

private:
	HuffMap map;
	HuffMapReverse rmap;
	frequencyMap & uniqueList;

	void setUpTree(std::vector<float> & values,
			const std::vector<HuffCode> & code) {
		std::vector<Node *> leaves;

		Node * root = createTree(leaves);

		calcCodesReverse(leaves);

		delete (root);

		values.resize(code.size());

//#pragma omp parallel for
		for (longValue i = 0; i < longValue(code.size()); ++i) {
			values[i] = *rmap[code[i]];
		}
	}

	void setUpTree(const std::vector<float> & values,
			std::vector<HuffCode *> & code) {
		binData(values);

		std::vector<Node *> leaves;

		Node * root = createTree(leaves);

		calcCodes(leaves);

		delete (root);

		code.resize(values.size());

#pragma omp parallel for
		for (longValue i = 0; i < longValue(values.size()); ++i) {
			code[i] = map[values[i]];
		}
	}

	void binData(const std::vector<float> & values) {
		frequencyMap ul;
		longValue i;
#pragma omp parallel for shared(ul) private(i) schedule(dynamic)
		for (i = 0; i < longValue(values.size()); ++i) {
			float val = values.at(i);

			if (ul.count(val) <= 0) {
#pragma omp critical
				{
					if (ul.count(val) <= 0)
						ul[val] = new longValue(1);
					else
						++(*ul[val]);
				}
			} else {
#pragma omp atomic
				++(*ul[val]);
			}
		}
		uniqueList = ul;
	}

	Node * createTree(std::vector<Node *>& leaf) {
		using namespace std;

		priority_queue<Node *, vector<Node *>, compareNode> que;

		for (frequencyMap::const_iterator it = uniqueList.begin();
				it != uniqueList.end(); ++it) {
			Node * n = new Node(new float(it->first), *it->second, NULL);
			que.push(n);
			leaf.push_back(n);
		}

		while (que.size() > 1) {
			Node * a = que.top();
			que.pop();
			Node * b = que.top();
			que.pop();

			Node * par = new Node(NULL, a->weight + b->weight, NULL);

			par->left = a;
			par->right = b;

			a->parent = par;
			b->parent = par;

			que.push(par);
		}

		return que.top();
	}

	void calcCodesReverse(const std::vector<Node *> & leaf) {
		using namespace std;

		for (std::vector<Node *>::const_iterator it = leaf.begin();
				it != leaf.end(); ++it) {

			Node * node = *it;
			Node * par = node->parent;

			HuffCode code;

			while (par->parent != NULL) {
				if (node == par->left)
					code.push_back(false);
				else
					code.push_back(true);

				node = par;
				par = par->parent;
			}
			if (node == par->left)
				code.push_back(false);
			else
				code.push_back(true);

			if (code.size() > 24) {
				union {
					float input;
					int output;
				} data;

				data.input = *(*it)->data;

				std::bitset<32> bits(data.output);

				code.clear();
				for (int i = 0; i < 32; ++i)
					code.push_back(bits[i] == 0 ? false : true);
			} else {
				for (int i = 0; i < int(code.size() % 8); ++i)
					code.push_back(false);
			}

			rmap[code] = new float(*(*it)->data);
		}
	}

	void calcCodes(const std::vector<Node *> & leaf) {
		using namespace std;

		for (std::vector<Node *>::const_iterator it = leaf.begin();
				it != leaf.end(); ++it) {

			Node * node = *it;
			Node * par = node->parent;

			HuffCode * code = new HuffCode[2];

			while (par->parent != NULL) {
				if (node == par->left)
					code[1].push_back(false);
				else
					code[1].push_back(true);

				node = par;
				par = par->parent;
			}
			if (node == par->left)
				code[1].push_back(false);
			else
				code[1].push_back(true);

			if (code[1].size() > 24) {
				union {
					float input;
					int output;
				} data;

				data.input = *(*it)->data;

				std::bitset<32> bits(data.output);

				code[1].clear();
				for (int i = 0; i < 32; ++i)
					code[1].push_back(bits[i] == 0 ? false : true);
				code[0].push_back(true);
				code[0].push_back(true);
			} else {
				for (int i = 0; i < int(code[1].size() % 8); ++i)
					code[1].push_back(false);

				if (code[1].size() == 8) {
					code[0].push_back(false);
					code[0].push_back(false);
				} else if (code[1].size() == 16) {
					code[0].push_back(false);
					code[0].push_back(true);
				} else {
					code[0].push_back(true);
					code[0].push_back(false);
				}
			}

			map[*(*it)->data] = code;
		}
	}
}
;

#endif /* HUFFMANTREE_H_ */
