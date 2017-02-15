#ifndef _POLYMESH_SKELETON2_MESH_H_
#define _POLYMESH_SKELETON2_MESH_H_

#include "Mesh.h"
#include "Skin2Joint.h"
#include "Skin2Vertex.h"
#include "Skin2Triangles.h"

#include <vector>

namespace pm
{

class Skin2Triangles;

class Skin2Mesh : public Mesh
{
public:
	Skin2Mesh(const std::vector<Skin2Joint>& joints, const std::vector<int>& vertices,
		const std::vector<sm::vec2>& texcoords, const std::vector<int>& triangles);
	virtual ~Skin2Mesh();

	virtual MeshType Type() const { return MESH_SKIN2; }

	virtual void Dump(std::vector<sm::vec2>& vertices, std::vector<sm::vec2>& texcoords,
		std::vector<int>& triangles) const;

	virtual void LoadFromTransform(const MeshTransform& transform);
	virtual void StoreToTransform(MeshTransform& transform) const;

	virtual const sm::vec2* GetVertexPos(int idx) const;
	virtual void SetVertexPos(int idx, const sm::vec2& pos);

	void Update(const float* (*query_joint_world_mt)(int joint_id, const void* ud), const void* ud);
	void Update(int offset0, int count0, int offset1, int count1, const float* vertices);

	const Skin2Triangles* GetMeshData() const { return m_mesh_data; }

private:
	Skin2Triangles* m_mesh_data;

}; // Skin2Mesh

}

#endif // _POLYMESH_SKELETON2_MESH_H_