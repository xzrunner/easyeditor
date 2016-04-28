#ifndef _EASYTERRAIN2D_MESH_SHAPE_H_
#define _EASYTERRAIN2D_MESH_SHAPE_H_

#include <easymesh.h>

namespace eterrain2d
{

class MeshShape : public emesh::Mesh
{
public:
	MeshShape(const ee::Symbol* base);

	void InsertTriangle(const ee::Vector* vertices, 
		const ee::Vector* texcoords, const std::vector<ee::Vector>& bound);

private:
	static const bool IS_BOUND = true;
	static const bool NOT_BOUND = false;

}; // MeshShape

}

#endif // _EASYTERRAIN2D_MESH_SHAPE_H_