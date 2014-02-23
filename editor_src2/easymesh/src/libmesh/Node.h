#ifndef _EASYMESH_NODE_H_
#define _EASYMESH_NODE_H_

namespace emesh
{

class Node : public d2d::Object
{
public:
	Node() {}
	Node(const Node& n) {
		src = n.src;
		dst = n.dst;
	}

public:
	d2d::Vector src, dst;

}; // Node

class NodeCmp
{
public:
	bool operator () (const Node* lhs, const Node* rhs) const {
		return lhs->src.x < rhs->src.x 
			|| lhs->src.x == rhs->src.x && lhs->src.y < rhs->src.y;
	}
}; // NodeCmp

}

#endif // _EASYMESH_NODE_H_