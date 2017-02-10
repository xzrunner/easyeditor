#ifndef _POLYMESH_TRIANGLES_MESH_H_
#define _POLYMESH_TRIANGLES_MESH_H_

#include "Mesh.h"
#include "Triangles.h"

#include <SM_Vector.h>

#include <vector>

namespace pm
{

class TrianglesMesh : public Mesh
{
public:
	TrianglesMesh(const std::vector<sm::vec2>& vertices, const std::vector<sm::vec2>& texcoords,
		const std::vector<int>& triangles);
	virtual ~TrianglesMesh();

	virtual MeshType Type() const { return MESH_TRIANGLES; }
	virtual const MeshData* GetMeshData() const { return m_mesh_data; }

	void Update(int offset0, int count0, int offset1, int count1, const float* vertices);

private:
	Triangles* m_mesh_data;

}; // TrianglesMesh

}

#endif // _POLYMESH_TRIANGLES_MESH_H_