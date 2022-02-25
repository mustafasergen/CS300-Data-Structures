#include "BVHTree.h"

void BVHTree::printNode(std::ostream &out, BVHTreeNode *node, int level) {
	if (root == nullptr) return;
	for (int i = 0; i < level; i++) {
		out << "  ";
	}
	if (!node->isLeaf) {
		out << "+ branch || ";
		node->aabb.printAABB(out);
		out << std::endl;
		printNode(out, node->rightChild, level + 1);
		printNode(out, node->leftChild, level + 1);
	}
	else {
		out << "- ";
		if (node->parent) {
			if (node->parent->rightChild == node)
				out << "R ";
			else
				out << "L ";
		}
		out << "- leaf: " << node->name << " || ";
		node->aabb.printAABB(out);
		out << std::endl;
	}
}
std::ostream &operator<<(std::ostream &out, BVHTree &tree) {
	tree.printNode(out, tree.root, 0);
	return out;
}

BVHTree::BVHTree()
{
	root = nullptr;
}
BVHTree::~BVHTree()
{
	deleter(root);
}

BVHTreeNode * BVHTree::deleter(BVHTreeNode * x)
{
	if(x)
	{
		x->rightChild = deleter(x->rightChild);
		x->leftChild = deleter(x->leftChild);
		delete x;
		return nullptr;
	}
	return nullptr;
}

BVHTreeNode * BVHTree::adder(AABB objectArea, std::string name, BVHTreeNode *x)
{
	if(x)
	{
		BVHTreeNode * node_to_add = new BVHTreeNode(objectArea,name,true);
		if(x->rightChild)
		{
			int increaseInRightTreeSize = AABB::unionArea(node_to_add->aabb,x->rightChild->aabb) - x->rightChild->aabb.getArea();
			int increaseInLeftTreeSize = AABB::unionArea(node_to_add->aabb,x->leftChild->aabb) - x->leftChild->aabb.getArea();
			if(increaseInRightTreeSize<increaseInLeftTreeSize)
			{
				x->rightChild = adder(objectArea,name,x->rightChild);
			}
			else
			{
				x->leftChild = adder(objectArea,name,x->leftChild);
			}
			return x;
		}
		else
		{
			BVHTreeNode * existing = x;
			BVHTreeNode * branchNode = new BVHTreeNode(existing->aabb+objectArea,"branch",false);
			branchNode->parent = existing->parent;
			branchNode->rightChild = existing;
			branchNode->rightChild->parent = branchNode;
			branchNode->leftChild = node_to_add;
			branchNode->leftChild->parent = branchNode;
			map[existing->name] = branchNode->rightChild;
			map[name] = branchNode->leftChild;
			return branchNode;
		}
	}
	else
	{
		x= new BVHTreeNode(objectArea,name,true);
		map[name] = x;
		return x;
	}
	
}

void BVHTree::adjustSize(BVHTreeNode * x)
{
	if(x)
	{
		adjustSize(x->leftChild);
		adjustSize(x->rightChild);
		if(!x->isLeaf)
		{
			x->aabb = x->leftChild->aabb + x->rightChild->aabb;
		}
	}
}


void BVHTree::collider(AABB object, BVHTreeNode *x, std::vector<std::string> &stored)
{
	if(x)
	{
		if(x->isLeaf)
		{
			if(x->aabb.collide(object))
				stored.push_back(x->name);
		}
		else
		{
			if(x->aabb.collide(object))
			{
				collider(object,x->leftChild,stored);
				collider(object,x->rightChild,stored);
			}
		}
		
	}
}
BVHTreeNode * BVHTree::getSibling(BVHTreeNode *x)
{
	if(x->parent)
	{
		if(x->parent->leftChild==x)
			return x->parent->rightChild;
		return x->parent->leftChild;
	}
	return nullptr;	
}
void BVHTree::addBVHMember(AABB objectArea, std::string name)
{
	root = adder(objectArea,name,root);
	adjustSize(root);
}
void BVHTree::moveBVHMember(std::string name, AABB newLocation)
{
	BVHTreeNode * x = map[name];
	if(x->parent)
	{
		if(newLocation+x->parent->aabb!=x->parent->aabb)
		{
			removeBVHMember(name);
			addBVHMember(newLocation,name);
		}
		else
		{
			x->aabb = newLocation;
		}
		
	}
	else
	{
		x->aabb = newLocation;
	}
	
}
void BVHTree::removeBVHMember(std::string name)
{
	BVHTreeNode * x = map[name];
	map.erase(name);
	if(x->parent)
	{
		BVHTreeNode * sibling = getSibling(x);
		x->parent->aabb = sibling->aabb;
		x->parent->isLeaf = sibling->isLeaf;
		x->parent->leftChild = sibling->leftChild;
		x->parent->rightChild = sibling->rightChild;
		x->parent->name = sibling->name;
		if(x->parent->isLeaf)
			map[x->parent->name] = x->parent;
		delete x;
		delete sibling;
	}
	else
	{
		root = nullptr;
		delete x;
	}
	
}
std::vector<std::string> BVHTree::getCollidingObjects(AABB object)
{
	std::vector<std::string> stored;
	collider(object,root,stored);
	return stored;
}