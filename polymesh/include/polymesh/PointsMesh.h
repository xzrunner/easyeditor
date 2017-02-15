#ifndef _POLYMESH_POINTS_MESH_H_
#define _POLYMESH_POINTS_MESH_H_

#include "Mesh.h"
#include "Triangles.h"

#include <SM_Vector.h>

#include <vector>

namespace pm
{

class Triangles;

class PointsMesh : public Mesh
{
public:
	PointsMesh(const std::vector<sm::vec2>& outline, const std::vector<sm::vec2>& points, 
		float width, float height);
	virtual ~PointsMesh();

	virtual MeshType Type() const { return MESH_POINTS; }

	virtual void Dump(std::vector<sm::vec2>& vertices, std::vector<sm::vec2>& texcoords,
		std::vector<int>& triangles) const;

	virtual void LoadFromTransform(const MeshTransform& transform);
	virtual void StoreToTransform(MeshTransform& transform) const;

	virtual const sm::vec2* GetVertexPos(int idx) const;
	virtual void SetVertexPos(int idx, const sm::vec2& pos);

	void Build(const std::vector<sm::vec2>& outline, const std::vector<sm::vec2>& points);
	void Clear();

	const std::vector<sm::vec2>& GetOutline() const { return m_outline; }
	const std::vector<sm::vec2>& GetPoints() const { return m_points; }

	const Triangles* GetMeshData() const { return m_mesh_data; }

private:
	float m_width, m_height;

	std::vector<sm::vec2> m_outline;
	std::vector<sm::vec2> m_points;	

	Triangles* m_mesh_data;

}; // PointsMesh

}

#endif // _POLYMESH_POINTS_MESH_H_