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

	virtual void Dump(std::vector<sm::vec2>& vertices, std::vector<sm::vec2>& texcoords,
		std::vector<int>& triangles) const;

	virtual void LoadFromTransform(const MeshTransform& transform);
	virtual void StoreToTransform(MeshTransform& transform) const;

	virtual const sm::vec2* GetVertexPos(int idx) const;
	virtual void SetVertexPos(int idx, const sm::vec2& pos);

	void Update(int offset0, int count0, int offset1, int count1, const float* vertices);

private:
	Triangles* m_mesh_data;

}; // TrianglesMesh

}

#endif // _POLYMESH_TRIANGLES_MESH_H_