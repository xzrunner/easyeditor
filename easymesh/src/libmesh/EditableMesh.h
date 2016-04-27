#ifndef _EASYMESH_EDITABLE_MESH_H_
#define _EASYMESH_EDITABLE_MESH_H_

#include "Mesh.h"

namespace ee { class Visitor; class Shape; class Vector; }

namespace emesh
{

class EditableMesh : public Mesh
{
public:
	EditableMesh() {}
	EditableMesh(const Mesh& mesh) 
		: Mesh(mesh) {}
	EditableMesh(const ee::Image& image) 
		: Mesh(image) {}
	virtual ~EditableMesh() {}

	virtual void TraverseMesh(ee::Visitor& visitor) const = 0;
	virtual bool RemoveMesh(ee::Shape* mesh) = 0;
	virtual bool InsertMesh(ee::Shape* mesh) = 0;
	virtual bool ClearMesh() = 0;	
	
	virtual void Reset() = 0;
	virtual void Clear() = 0;

}; // EditableMesh

}

#endif // _EASYMESH_EDITABLE_MESH_H_