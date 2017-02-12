#ifndef _EASYTERRAIN2D_MESH_SHAPE_H_
#define _EASYTERRAIN2D_MESH_SHAPE_H_

#include <easymesh.h>

namespace eterrain2d
{

class MeshShape : public emesh::Mesh
{
public:
	MeshShape(const ee::Symbol* base);

	virtual pm::MeshType Type() const { return s2::MESH_UNKNOWN; }

	void InsertTriangle(const sm::vec2* vertices, 
		const sm::vec2* texcoords, const std::vector<sm::vec2>& bound);

private:
	static const bool IS_BOUND = true;
	static const bool NOT_BOUND = false;

}; // MeshShape

}

#endif // _EASYTERRAIN2D_MESH_SHAPE_H_