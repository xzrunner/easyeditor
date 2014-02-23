#ifndef _EASYMESH_NODE_H_
#define _EASYMESH_NODE_H_

namespace emesh
{

class Node : public d2d::Object
{
public:
	Node() {}
	Node(const d2d::Vector& pos, int width, int height) {
		xy = pos;
 		uv.x = pos.x / width + 0.5f;
 		uv.y = pos.y / height + 0.5f;
	}
	Node(const Node& n) {
		uv = n.uv;
		xy = n.xy;
	}

public:
	static const int RADIUS = 5;

public:
	d2d::Vector uv, xy;

}; // Node

class NodeCmp
{
public:
	bool operator () (const Node* lhs, const Node* rhs) const {
		return lhs->uv.x < rhs->uv.x 
			|| lhs->uv.x == rhs->uv.x && lhs->uv.y < rhs->uv.y;
	}
}; // NodeCmp

}

#endif // _EASYMESH_NODE_H_