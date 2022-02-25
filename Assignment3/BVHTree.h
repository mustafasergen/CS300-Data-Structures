#include "AABB.h"
#include "BVHTreeNode.h"
#include <vector>
#include <iostream>
#include <unordered_map>
#ifndef _BVH_TREE
#define _BVH_TREE

class BVHTree {
private:
	BVHTreeNode *root;
	std::unordered_map<std::string, BVHTreeNode *> map;
public:
	BVHTree();
	~BVHTree();
	BVHTreeNode * deleter(BVHTreeNode * x);
	void printNode(std::ostream &out, BVHTreeNode *, int level);
	BVHTreeNode * adder(AABB objectArea, std::string name, BVHTreeNode * x);
	void collider(AABB object, BVHTreeNode * x, std::vector<std::string> & stored);
	BVHTreeNode * getSibling(BVHTreeNode * x);
	void addBVHMember(AABB objectArea, std::string name);
	void moveBVHMember(std::string name, AABB newLocation);
	void removeBVHMember(std::string name);
	std::vector<std::string> getCollidingObjects(AABB object);
	void adjustSize(BVHTreeNode * x);
	friend std::ostream &operator<<(std::ostream &out, BVHTree &tree);
};

#endif