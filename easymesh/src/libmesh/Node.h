#ifndef _EASYMESH_NODE_H_
#define _EASYMESH_NODE_H_

#include <ee/Object.h>
#include <ee/SelectionSet.h>

namespace emesh
{

class Node : public ee::Object
{
public:
	Node() : ud(NULL) {}
	Node(const sm::vec2& pos, int width, int height) 
		: ud(NULL) {
		xy = ori_xy = pos;
 		uv.x = pos.x / width + 0.5f;
 		uv.y = pos.y / height + 0.5f;
	}
	Node(const sm::vec2& _xy, const sm::vec2& _uv) 
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
	sm::vec2 uv, xy;
	sm::vec2 ori_xy;

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