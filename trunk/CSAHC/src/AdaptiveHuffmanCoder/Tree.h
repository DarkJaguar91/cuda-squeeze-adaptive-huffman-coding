/*
 * Tree.h
 *
 *  Created on: Jun 25, 2013
 *      Author: Brandon
 */

#ifndef TREE_H_
#define TREE_H_

#include <cstdlib>
#include <vector>
#include <iostream>

template<class T>
struct Node {
	Node(T * d, Node * par = NULL) :
			data(d), weight(0), parent(par), left(NULL), right(NULL) {
	}

	~Node() {
		delete (data);
	}

	T * data;
	int weight;
	Node<T> * parent;
	Node<T> * left;
	Node<T> * right;
};

template<class T>
class HTree {
public:
	HTree(int max) :
			maxNodes(max), root(NULL) {
	}

	void print() {
		printFromNode(root, 0);
	}

	void printFromNode(Node<T> * n, int depth) {
		using namespace std;

		for (int i = 0; i < depth; ++i)
			cout << "\t";

		if (n != NULL) {
			if (n->data == NULL)
				cout << "(joiner" << " - " << n->weight << ")\n";
			else
				cout << "(" << *(n->data) << " - " << n->weight << ")\n";

			printFromNode(n->left, depth + 1);
			printFromNode(n->right, depth + 1);
		} else {
			cout << "(NULL)\n";
		}
	}

	void encode(T data) {
		using namespace std;
		if (root == NULL) {
//			cout << "root was null, adding " << data << endl;
			root = new Node<T>(NULL);
			root->right = new Node<T>(new T(data), root);
			++root->right->weight;

			update(root);
			return;
		}

		Node<T> * n = findNode(root, data);

		if (n == NULL) { // element not found
//			cout << "element not found adding " << data << endl;
			n = findNULLParent(root);

			n->left = new Node<T>(NULL, n);
			n->left->right = new Node<T>(new T(data), n->left);
			++n->left->right->weight;

			update(n->left);
		} else { // element exists
			update(n);
		}
	}

	void update(Node<T> * n) {
		using namespace std;

		Node<T> * tmp = getMostSig(root, n->weight);

		if (tmp != n){
			if (tmp->data != NULL){
				exchangeNodes(n, tmp);
			}
		}

		++n->weight;
		if (n->parent != NULL) // not root
			update(n->parent);
	}

private:
	int maxNodes;
	Node<T> * root;

	std::vector<std::vector<Node<T> *> > blocks;

	void exchangeNodes(Node<T> * n1, Node<T> * n2) {
		if (n1->parent->left == n1) {
			if (n2->parent->left == n2) {
				std::swap(n1->parent->left, n2->parent->left);
			} else {
				std::swap(n1->parent->left, n2->parent->right);
			}
		} else {
			if (n2->parent->left == n2) {
				std::swap(n1->parent->right, n2->parent->left);
			} else {
				std::swap(n1->parent->right, n2->parent->right);
			}
		}

		std::swap(n1->parent, n2->parent);
	}

	Node<T> * getMostSig(Node<T> * n, int weight) {
		using namespace std;

		vector<Node<T> *> que;

		que.push_back(n);

		while (que.size() > 0) {
			n = que.at(0);
			que.erase(que.begin());

			if (n->weight == weight)
				return n;

			if (n->right != NULL)
				que.push_back(n->right);
			if (n->left != NULL)
				que.push_back(n->left);
		}
		return NULL;
	}

	Node<T> * findNULLParent(Node<T> * n) {
		if (n->right == NULL) {
			return NULL;
		} else if (n->left == NULL) {
			return n;
		} else {
			Node<T> * save = findNULLParent(n->left);
			if (save == NULL)
				return findNULLParent(n->right);
			return save;
		}
	}

	Node<T> * findNode(Node<T> * n, T data) {
		if (n == NULL) {
			return NULL;
		} else if (n->data == NULL) {
			Node<T> * save = findNode(n->right, data);
			if (save == NULL)
				return findNode(n->left, data);
			return save;
		} else if (*(n->data) == data) {
			return n;
		} else {
			Node<T> * save = findNode(n->right, data);
			if (save == NULL)
				return findNode(n->left, data);
			return save;
		}
	}
};

#endif /* TREE_H_ */
