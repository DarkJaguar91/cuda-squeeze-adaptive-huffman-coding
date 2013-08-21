/*
 * HTree.h
 *
 *  Created on: Aug 21, 2013
 *      Author: Brandon
 */
#include <iostream>
#include <queue>
#include <map>
#include <iterator>
#include <algorithm>
#include <math.h>
#include <time.h>
#include <algorithm>
#include <omp.h>

#ifndef HTREE_H_
#define HTREE_H_

typedef std::vector<bool> HuffCode;
typedef std::map<float, int> frequencyTable;
typedef std::map<float, HuffCode> HuffCodeMap;
typedef std::map<HuffCode, float> HuffCodeMap2;

class INode {
public:
	const int f;

	virtual ~INode() {
	}

protected:
	INode(int f) :
			f(f) {
	}
};

class InternalNode: public INode {
public:
	INode * const left;
	INode * const right;

	InternalNode(INode* c0, INode* c1) :
			INode(c0->f + c1->f), left(c0), right(c1) {
	}
	~InternalNode() {
		delete left;
		delete right;
	}
};

class LeafNode: public INode {
public:
	const float c;

	LeafNode(int f, float c) :
			INode(f), c(c) {
	}
};

struct NodeCmp {
	bool operator()(const INode* lhs, const INode* rhs) const {
		return lhs->f > rhs->f;
	}
};

class HTreez {
public:
	HTreez(){
	}

	void createCodes(const std::vector<float> &numbers, HuffCode& code) {
		binData(numbers);

		INode * root = BuildTree(uniqueValues);

		GenerateCodes(root, HuffCode(), map); //, map2);

		delete (root);

		for (std::vector<float>::const_iterator it = numbers.begin();
				it != numbers.end(); ++it) {
			HuffCode c = map[*it];
			std::copy(c.begin(), c.end(), std::back_inserter(code));
		}
	}

//	void decode(const HuffCode& code, std::vector<float> &output){
//		HuffCode tmpCode;
//
//		for (HuffCode::const_iterator it = code.begin(); it != code.end(); ++it){
//			tmpCode.push_back(*it);
//			if (map2.count(tmpCode) > 0){
//				output.push_back(map2[tmpCode]);
//				tmpCode.clear();
//			}
//		}
//	}

	~HTreez() {
	}

	void printCodes() {
		for (HuffCodeMap::const_iterator it = map.begin(); it != map.end();
				++it) {
			std::cout << it->first << " ";
			std::copy(it->second.begin(), it->second.end(),
					std::ostream_iterator<bool>(std::cout));
			std::cout << std::endl;
		}
	}

private:
	HuffCodeMap map;
	frequencyTable uniqueValues;
//	HuffCodeMap2 map2;

	void binData(const std::vector<float> &numbers) {
		frequencyTable ul;
		int i;
#pragma omp parallel for shared(ul) private(i) schedule(dynamic)
		for (i = 0; i < int(numbers.size()); ++i) {
			float val = numbers.at(i);

			if (ul.count(val) <= 0) {
#pragma omp critical
				{
					if (ul.count(val) <= 0)
						ul[val] = 1;
					else
						++ul.find(val);
				}
			} else {
#pragma omp atomic
				ul[val] += 1;
			}
		}
		uniqueValues = ul;
	}

	INode* BuildTree(const frequencyTable &frequencies) {
		std::priority_queue<INode*, std::vector<INode*>, NodeCmp> trees;

		for (frequencyTable::const_iterator it = frequencies.begin();
				it != frequencies.end(); ++it) {
			trees.push(new LeafNode((int) it->second, (float) it->first));
		}
		while (trees.size() > 1) {
			INode* childR = trees.top();
			trees.pop();

			INode* childL = trees.top();
			trees.pop();

			INode* parent = new InternalNode(childR, childL);
			trees.push(parent);
		}
		return trees.top();
	}

	void GenerateCodes(const INode* node, const HuffCode& prefix,
			HuffCodeMap& outCodes) //, HuffCodeMap2& outCodes2)
			{
		if (const LeafNode* lf = dynamic_cast<const LeafNode*>(node)) {
			outCodes[lf->c] = prefix;
//	        outCodes2[prefix] = lf->c;
		} else if (const InternalNode* in =
				dynamic_cast<const InternalNode*>(node)) {
			HuffCode leftPrefix = prefix;
			leftPrefix.push_back(false);
			GenerateCodes(in->left, leftPrefix, outCodes); //, outCodes2);

			HuffCode rightPrefix = prefix;
			rightPrefix.push_back(true);
			GenerateCodes(in->right, rightPrefix, outCodes); //, outCodes2);
		}
	}
};

#endif /* HTREE_H_ */
