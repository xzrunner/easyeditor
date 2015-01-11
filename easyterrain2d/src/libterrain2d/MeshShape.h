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
		const d2d::Vector* texcoords);

	

}; // MeshShape

}

#endif // _EASYTERRAIN2D_MESH_SHAPE_H_