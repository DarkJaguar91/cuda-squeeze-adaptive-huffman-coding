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
	HTree() :
			root(NULL) {
	}

	HTree(std::vector<T> start) :
			root(NULL) {
		if (start.size() > 0) {
			firstFoundValues = start;
		}
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

	std::vector<T> decode(std::vector<int> & values) {
		using namespace std;

		Node<T> * tmp = root;

		vector<T> output;

		// if root is null
		if (tmp == NULL) {
			cout << "root was null\n";
			cout << "looking at value: " << values.at(0) << endl;

			root = new Node<T>(NULL);
			root->right = new Node<T>(new T(firstFoundValues.at(0)), root);
			++root->right->weight;

			update(root);
			output.push_back(values.at(0));

			firstFoundValues.erase(firstFoundValues.begin());
			values.erase(values.begin()); // first value always placed first
		}

		tmp = root;

		print();

		while (values.size() > 0) {
			cout << "looking at value: " << values.at(0) << " with "
					<< values.size()-1 << " Values left" << endl;

			Node<T> * lookAt = NULL;

			if (values.at(0) == 0) {
				cout << "LEFT" << endl;
				lookAt = tmp->left;
			} else {
				cout << "Right" << endl;
				lookAt = tmp->right;
			}

			if (lookAt == NULL){ // value not found.. add it
				cout << "value not found. Adding it" << endl;

				tmp->left = new Node<T>(NULL, tmp);
				tmp->left->right = new Node<T>(new T(firstFoundValues.at(0)), tmp->left);
				++tmp->left->right->weight;
				update(tmp->left);

				tmp = root;
				output.push_back(firstFoundValues.at(0));
				firstFoundValues.erase(firstFoundValues.begin());
				values.erase(values.begin());
			}
			else if (lookAt->data != NULL){
				cout << "found a value, adding it\n";
				output.push_back(*lookAt->data);
				update(lookAt);
				tmp = root;
			}
			else {
				tmp = lookAt;
			}

			values.erase(values.begin());
		}

		return output;
	}

	std::vector<int> encode(T data) {
		using namespace std;
		if (root == NULL) {
//			cout << "root was null, adding " << data << endl;
			root = new Node<T>(NULL);
			root->right = new Node<T>(new T(data), root);
			++root->right->weight;

			update(root);
			vector<int> o;
			o.push_back(1);

			firstFoundValues.push_back(data);

			return o;
		}

		Node<T> * n = findNode(root, data);

		vector<int> code;

		if (n == NULL) { // element not found
//			cout << "element not found adding " << data << endl;
			n = findNULLParent(root);

			n->left = new Node<T>(NULL, n);
			n->left->right = new Node<T>(new T(data), n->left);
			++n->left->right->weight;

			code = getCode(n->left->right);
			update(n->left);

			firstFoundValues.push_back(data);
		} else { // element exists
			code = getCode(n);
			update(n);
		}
		return code;
	}

	std::vector<int> getCode(Node<T> * n) {
		using namespace std;

		vector<int> out;

		Node<T> * par = n->parent;

		while (par->parent != NULL) {
			if (par->left == n)
				out.push_back(0);
			else
				out.push_back(1);

			n = par;
			par = par->parent;
		}
		if (par->left == n) // root value
			out.push_back(0);
		else
			out.push_back(1);

		std::reverse(out.begin(), out.end());

		return out;
	}

	void update(Node<T> * n) {
		using namespace std;

		if (n->data != NULL) {
			Node<T> * tmp = getMostSig(root, n->weight);

			if (tmp != n) {
				exchangeNodes(n, tmp);
			}
		} else {
			if (n->parent != NULL) {
				Node<T> * par = n->parent;
				if (par->left->weight >= par->right->weight) {
					exchangeNodes(par->left, par->right);
				}
			}
		}

		++n->weight;

		if (n->parent != NULL) // not root
			update(n->parent);
	}

	std::vector<T> getFirstValueList() {
		return firstFoundValues;
	}

private:
	Node<T> * root;

	std::vector<T> firstFoundValues;

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
