#include "Skin2Mesh.h"

namespace pm
{

Skin2Mesh::Skin2Mesh(const std::vector<Skin2Vertex::Part>& parts,
					 const std::vector<int>& vertices,
					 const std::vector<sm::vec2>& texcoords,
					 const std::vector<int>& triangles)
{
	m_mesh_data = Skin2Triangles::Create(parts, vertices, texcoords, triangles);
}

Skin2Mesh::~Skin2Mesh()
{
	delete m_mesh_data;
}

void Skin2Mesh::Update(const float* (*query_joint_world_mt)(int joint_id))
{
	for (int i = 0; i < m_mesh_data->m_vert_num; ++i)
	{
		sm::vec2 new_xy(0, 0);
		Skin2Vertex* vertex = m_mesh_data->m_vertices[i];
		for (int j = 0; j < vertex->num; ++j) 
		{
			const Skin2Vertex::Part& p = m_mesh_data->m_parts[vertex->parts[j]];
			const float* mat = query_joint_world_mt(p.joint_id);
			sm::vec2 v = p.vertex + p.offset;
			new_xy.x += (v.x * mat[0] + v.y * mat[2] + mat[4]) * p.weight;
			new_xy.y += (v.x * mat[1] + v.y * mat[3] + mat[5]) * p.weight;
		}
		vertex->xy = new_xy;
	}
}

void Skin2Mesh::Update(int offset0, int count0, 
						   int offset1, int count1, 
						   const float* vertices)
{
 	for (int i = 0; i < count0; ++i) {
 		m_mesh_data->m_parts[offset0 + i].offset.Set(0, 0);
 	}
 	for (int i = 0; i < count1; ++i) {
 		m_mesh_data->m_parts[offset1 + i].offset.Set(0, 0);
 	}
 
 	int ptr = 0;
 	for (int i = 0; i < count0; ++i) {
 		sm::vec2 offset(vertices[ptr++], vertices[ptr++]);
 		m_mesh_data->m_parts[offset0 + i].offset += offset;
 	}
 	for (int i = 0; i < count1; ++i) {
 		sm::vec2 offset(vertices[ptr++], vertices[ptr++]);
 		m_mesh_data->m_parts[offset1 + i].offset += offset;
 	}
}

}