#ifndef _EASYTERRAIN2D_MESH_SHAPE_H_
#define _EASYTERRAIN2D_MESH_SHAPE_H_

#include <easymesh.h>

namespace eterrain2d
{

class MeshShape : public emesh::Shape
{
public:
	MeshShape(const d2d::Image& image);

	void InsertTriangle(const d2d::Vector* vertices, 
		const d2d::Vector* texcoords, const std::vector<d2d::Vector>& bound);

private:
	static const bool IS_BOUND = true;
	static const bool NOT_BOUND = false;

}; // MeshShape

}

#endif // _EASYTERRAIN2D_MESH_SHAPE_H_