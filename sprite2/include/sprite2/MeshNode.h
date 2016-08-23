#ifndef _SPRITE2_MESH_NODE_H_
#define _SPRITE2_MESH_NODE_H_

#include <CU_RefCountObj.h>
#include <SM_Vector.h>

namespace s2
{

class MeshNode : public cu::RefCountObj
{
public:
	MeshNode() 
		: ud(NULL) 
	{}
	MeshNode(const sm::vec2& pos, int width, int height) 
		: ud(NULL) 
	{
		xy = ori_xy = pos;
 		uv.x = pos.x / width + 0.5f;
 		uv.y = pos.y / height + 0.5f;
	}
	MeshNode(const sm::vec2& _xy, const sm::vec2& _uv) 
		: ud(NULL) 
	{
		xy = ori_xy = _xy;
		uv = _uv;
	}
	MeshNode(const MeshNode& n) 
		: ud(NULL) 
	{
		uv = n.uv;
		xy = ori_xy = n.xy;
	}

public:
	sm::vec2 uv, xy;
	sm::vec2 ori_xy;

	void* ud;

}; // MeshNode

class NodeCmp
{
public:
	bool operator () (const MeshNode* lhs, const MeshNode* rhs) const {
		return lhs->uv.x < rhs->uv.x 
			|| lhs->uv.x == rhs->uv.x && lhs->uv.y < rhs->uv.y;
	}
}; // NodeCmp

}

#endif // _SPRITE2_MESH_NODE_H_