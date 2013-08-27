/*
 * HTree.h
 *
 *  Created on: 27 Aug 2013
 *      Author: bjtal
 */

#include <iostream>
#include <queue>
#include <map>
#include <algorithm>

#ifndef HTREE_H_
#define HTREE_H_

typedef unsigned long long longValue;
typedef std::vector<bool> HuffCode;
typedef std::map<float, longValue*> frequencyValues;

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

struct NodeCompare {
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

class HTree {
public:
	HTree(frequencyValues & freqMap) :
			frequencyMap(freqMap) {
	}
	;
	virtual ~HTree() {
	}
	;

protected:
	frequencyValues & frequencyMap;

	Node * createTree(std::vector<Node *>& leaf) {
		using namespace std;

		priority_queue<Node *, vector<Node *>, NodeCompare> que;

		for (frequencyValues::const_iterator it = frequencyMap.begin();
				it != frequencyMap.end(); ++it) {
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
};

#endif /* HTREE_H_ */
