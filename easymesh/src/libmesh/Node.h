#ifndef _EASYMESH_NODE_H_
#define _EASYMESH_NODE_H_

#include <ee/Object.h>
#include <ee/Vector.h>
#include <ee/SelectionSet.h>

namespace emesh
{

class Node : public ee::Object
{
public:
	Node() : ud(NULL) {}
	Node(const ee::Vector& pos, int width, int height) 
		: ud(NULL) {
		xy = ori_xy = pos;
 		uv.x = pos.x / width + 0.5f;
 		uv.y = pos.y / height + 0.5f;
	}
	Node(const ee::Vector& _xy, const ee::Vector& _uv) 
		: ud(NULL) {
		xy = ori_xy = _xy;
		uv = _uv;
	}
	Node(const Node& n) 
		: ud(NULL) {
		uv = n.uv;
		xy = ori_xy = n.xy;
	}

public:
	ee::Vector uv, xy;
	ee::Vector ori_xy;

	void* ud;

}; // Node

class NodeCmp
{
public:
	bool operator () (const Node* lhs, const Node* rhs) const {
		return lhs->uv.x < rhs->uv.x 
			|| lhs->uv.x == rhs->uv.x && lhs->uv.y < rhs->uv.y;
	}
}; // NodeCmp

typedef ee::SelectionSet<Node> NodeSelection;

}

#endif // _EASYMESH_NODE_H_