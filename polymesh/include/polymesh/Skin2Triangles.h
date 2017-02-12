#ifndef _POLYMESH_SKIN2_TRIANGLES_H_
#define _POLYMESH_SKIN2_TRIANGLES_H_

#include "MeshData.h"
#include "Skin2Vertex.h"

#include <stdint.h>

namespace pm
{

class Skin2Triangles : public MeshData
{
public:
	Skin2Triangles();

	virtual void Dump(std::vector<sm::vec2>& vertices, std::vector<sm::vec2>& texcoords,
		std::vector<int>& triangles) const;

	virtual void LoadFromTransform(const MeshTransform& transform);
	virtual void StoreToTransform(MeshTransform& transform) const;

	static Skin2Triangles* Create(const std::vector<Skin2Vertex::Part>& parts,
		                          const std::vector<int>& vertices,
								  const std::vector<sm::vec2>& texcoords,
								  const std::vector<int>& triangles);

private:
	uint16_t m_joint_num;
	uint16_t m_vert_num;
	uint16_t m_tri_num;
	uint16_t m_vertices_joint_num;

	Skin2Vertex* m_vertices;
	uint16_t*    m_vertices_joint;

	uint16_t*    m_triangles;

	Skin2Joint   m_joints[1];	

	friend class Skin2Mesh;

}; // Skin2Triangles

}

#endif // _POLYMESH_SKIN2_TRIANGLES_H_