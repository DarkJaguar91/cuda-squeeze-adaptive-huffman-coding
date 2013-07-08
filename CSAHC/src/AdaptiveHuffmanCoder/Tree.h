/*
 * Tree.h
 *
 *  Created on: Jun 25, 2013
 *      Author: Brandon James Talbot
 *
 *      Huffman tree to both encode values 1 at a time, and decode a long string of values
 */

#ifndef TREE_H_
#define TREE_H_

#include <cstdlib>
#include <vector>
#include <iostream>
#include <algorithm>


typedef unsigned char byte;

/**
 * Node
 * templated
 * Holds all data each node in the tree requires
 */
template<class T>
struct Node {

	/**
	 * Constructor
	 * d the data for the node
	 * par the parent node
	 */
	Node(T * d, Node * par = NULL, long w = 0, long h = 0) :
			data(d), weight(w), height(h), parent(par), left(NULL), right(NULL) {
	}

	/**
	 * Destructor
	 */
	~Node() {
		delete (data);
	}

	T * data; // data ascosiated to the node
	long weight; // the number of times the value has been seen
	long height;
	Node<T> * parent; // the parent node
	Node<T> * left; // left child node
	Node<T> * right; // right child node
};

/**
 * Tree class to decode and find codes for Huffman Coding
 */
template<class T>
class HTree {
public:
	/**
	 * Constructor
	 * Default
	 * Sets the root to NULL only
	 */
	HTree() :
			root(NULL), nullParent(NULL) {
	}

	/**
	 * Constructor
	 * For decoding
	 * Sets the initial code values list
	 * and sets root to NULL
	 */
	HTree(std::vector<T> start) :
			root(NULL), nullParent(NULL) {
		if (start.size() > 0) {
			uniqueValueOrderedList = start;
		}
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
	void printFromNode(Node<T> * n, long depth) {
		using namespace std;

//		for (long i = 0; i < depth; ++i)
//			cout << "\t";

		if (n != NULL) {

			if (depth != n->height){
				cout << "failed expected " << depth << " had " << n->height << endl;
			}
//			if (n->data == NULL)
//				cout << "(joiner" << " - " << n->weight << ")\n";
//			else
//				cout << "(" << *(n->data) << " - " << n->weight << ")\n";

			printFromNode(n->left, depth + 1);
			printFromNode(n->right, depth + 1);
		} else {
//			cout << "(NULL)\n";
		}
	}

	/**
	 * Decode the list of numbers
	 */
	std::vector<T> decode(std::vector<byte> & values, long numValues) {
		using namespace std;

		Node<T> * tmp = root; // set a temp node to root

		vector<T> output; // the output list of numbers initialiser

		// if root is null
		if (tmp == NULL) {
			root = new Node<T>(NULL, NULL, 0, 0); // create the root node
			root->right = new Node<T>(new T(uniqueValueOrderedList.at(0)), root, root->height + 1); // create the first data node with the first found value


			update(root); // update the root node (inc weight)

			output.push_back(uniqueValueOrderedList.at(0)); // get the value

			uniqueValueOrderedList.erase(uniqueValueOrderedList.begin()); // delete already used value

			values.erase(values.begin()); // when adding new value, we have to delete 2 markers
		}

		tmp = root; // reset the tmp value to root

		while (values.size() > 0) { // while we still have values to read
			if (long(output.size()) == numValues)
				return output;
			Node<T> * lookAt = NULL; // create a temp child node

			if (values.at(0) == 0) { // move to left child if placer is 0
				lookAt = tmp->left;
			} else if (values.at(0) == 1) {// move to right child if placer is 1
				lookAt = tmp->right;
			}
			else {
				break;
			}

			if (lookAt == NULL){ // value not found.. add it
				tmp->left = new Node<T>(NULL, tmp); // add the child that is missing
				tmp->left->right = new Node<T>(new T(uniqueValueOrderedList.at(0)), tmp->left, 1); // get the value from the list
				update(tmp->left); // upate the tree from the parent

				tmp = root; // finished with this value, reset our temp node to root
				output.push_back(uniqueValueOrderedList.at(0)); // get the value of the node
				uniqueValueOrderedList.erase(uniqueValueOrderedList.begin()); // delete used value from our new value list
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

	/**
	 * Encode a value
	 */
	std::vector<byte> encode(T data) {
		using namespace std;
		if (root == NULL) { // if the tree is new (root is null)
			root = new Node<T>(NULL); // set the root node
			root->right = new Node<T>(new T(data), root, 1, 1); // set the first value

			update(root); // update the root node (for weight)
			vector<byte> o; // create the code
			o.push_back(1);

			uniqueValueOrderedList.push_back(data); // new value was found, add it to our unique value list

			nullParent = root;

			return o; // return our code for this value
		}

		Node<T> * n = findNode(root, data); // look for the value in our tree

		vector<byte> code; // temp value for our code

		if (n == NULL) { // element not found in tree
			n = nullParent;//findNULLParent(root); // find the parent of the null child

			n->left = new Node<T>(NULL, n, 0, n->height + 1); // create the new joiner
			n->left->right = new Node<T>(new T(data), n->left, 1, n->height + 2); // add the data leaf node

			code = getCode(n->left->right); // get the code for the node
			update(n->left); // update the tree from parent node

			nullParent = n->left;

			uniqueValueOrderedList.push_back(data); // new value found, add it to our unique value list
		} else { // element exists in tre
			code = getCode(n); // get the code for the element
			update(n); // update tree from the node
		}
		return code; // return the found code
	}

	/**
	 * Gets the code of the node
	 */
	std::vector<byte> getCode(Node<T> * n) {
		using namespace std;

		vector<byte> out; // the code temp value

		Node<T> * par = n->parent; // get the parent of the node

		while (par->parent != NULL) { // if the node is not the root node
			if (par->left == n) // is it the left child
				out.push_back(0); // add 0 to code
			else
				out.push_back(1); // right child adds 1 to code

			n = par; // change pointer to parent node
			par = par->parent; // change pointer to parent node
		}
		if (par->left == n) // root value
			out.push_back(0); // 0 for left child
		else
			out.push_back(1); // 1 for right child

		std::reverse(out.begin(), out.end()); // reverse the code as we went backwards

		return out; // return the code
	}

	/**
	 * Update the tree from node
	 */
	void update(Node<T> * n) {
		using namespace std;

		Node<T> * tmp = getMostSig(root, n->weight);

		if (n != tmp){
			if (n->parent != tmp){
				exchangeNodes(n, tmp);
			}
		}

		++n->weight; // increase the weight of the node

		if (n->parent != NULL) // not root
			update(n->parent); // update from parent
	}

	/**
	 * Getter for list of unique values
	 */
	std::vector<T> getUniqueValueOrderedList() {
		return uniqueValueOrderedList;
	}

	long getNumberValues(){
		return root->weight;
	}

private:
	Node<T> * root; // the root nodes pointer
	Node<T> * nullParent;
//	std::vector< std::vector<Node<T> * > > blockList;

	std::vector<T> uniqueValueOrderedList; // the unique list

	/**
	 * Swap to nodes parents and the parents children
	 */
	void exchangeNodes(Node<T> * n1, Node<T> * n2) {
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

		std::swap(n1->parent, n2->parent); // swap the parent pointers
	}

	/**
	 * find the highest node with specified weight from node N
	 */
	Node<T> * getMostSig(Node<T> * n, long weight) {
		using namespace std;

		vector<Node<T> *> que; // que for breadth tree searching

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

	/**
	 * find the node of the null node
	 */
	Node<T> * findNULLParent(Node<T> * n) {
		if (n->right == NULL) { // if right is null
			return NULL; // wrong node return NULL
		} else if (n->left == NULL) { // if left is null
			return n; // found our node, return it
		} else {
			Node<T> * save = findNULLParent(n->left); // recursively check left sub tree
			if (save == NULL) // if not found in left sub tree
				return findNULLParent(n->right); // check right sub tree
			return save; // return what was found (NULL if not found)
		}
	}

	/**
	 * Find node with data specified
	 */
	Node<T> * findNode(Node<T> * n, T data) {
		if (n == NULL) { // if null node
			return NULL; // wrong node return null
		} else if (n->data == NULL) { //if not a leaf node
			Node<T> * save = findNode(n->right, data); // check right sub tree (right first cus leaves are more likely on the right)
			if (save == NULL) // if not in right sub tree
				return findNode(n->left, data); // check left sub tree
			return save; // return what was found (NULL if not in sub trees)
		} else if (*(n->data) == data) { // if data is the one were looking for
			return n; // return the node
		} else { // not found return null
			return NULL;
		}
	}
};

#endif /* TREE_H_ */
