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
	virtual const MeshData* GetMeshData() const { return m_mesh_data; }

private:
	std::vector<sm::vec2> m_outline;
	std::vector<sm::vec2> m_points;	

	Triangles* m_mesh_data;

}; // PointsMesh

}

#endif // _POLYMESH_POINTS_MESH_H_