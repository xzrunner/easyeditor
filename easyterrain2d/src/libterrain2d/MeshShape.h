#ifndef _EASYTERRAIN2D_MESH_SHAPE_H_
#define _EASYTERRAIN2D_MESH_SHAPE_H_

#include <easymesh.h>

namespace eterrain2d
{

class MeshShape : public emesh::Mesh
{
public:
	MeshShape(const ee::Symbol* base);

	void InsertTriangle(const sm::vec2* vertices, 
		const sm::vec2* texcoords, const std::vector<sm::vec2>& bound);

	virtual int PointQueryVertex(const sm::vec2& p) const { return -1; }
	virtual void RectQueryVertices(const sm::rect& r, std::vector<int>& vertices) const {}

private:
	static const bool IS_BOUND = true;
	static const bool NOT_BOUND = false;

}; // MeshShape

}

#endif // _EASYTERRAIN2D_MESH_SHAPE_H_