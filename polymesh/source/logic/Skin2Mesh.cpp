#include "Skin2Mesh.h"
#include "Skin2TrisHelper.h"

namespace pm
{

Skin2Mesh::Skin2Mesh(const std::vector<Skin2Joint>& joints,
					 const std::vector<int>& vertices,
					 const std::vector<sm::vec2>& texcoords,
					 const std::vector<int>& triangles)
{
	m_mesh_data = Skin2Triangles::Create(joints, vertices, texcoords, triangles);
}

Skin2Mesh::~Skin2Mesh()
{
	delete m_mesh_data;
}

void Skin2Mesh::Dump(std::vector<sm::vec2>& vertices, 
					 std::vector<sm::vec2>& texcoords, 
					 std::vector<int>& triangles) const
{
	Skin2TrisHelper::Dump(*m_mesh_data, vertices, texcoords, triangles);
}

void Skin2Mesh::LoadFromTransform(const MeshTransform& transform)
{
	Skin2TrisHelper::LoadFromTransform(*m_mesh_data, transform);
}

void Skin2Mesh::StoreToTransform(MeshTransform& transform) const
{
	Skin2TrisHelper::StoreToTransform(transform, *m_mesh_data);
}

const sm::vec2* Skin2Mesh::GetVertexPos(int idx) const
{
	return Skin2TrisHelper::GetVertexPos(*m_mesh_data, idx);
}

void Skin2Mesh::SetVertexPos(int idx, const sm::vec2& pos)
{
	Skin2TrisHelper::SetVertexPos(*m_mesh_data, idx, pos);
}

void Skin2Mesh::Update(const float* (*query_joint_world_mt)(int joint_id, const void* ud), const void* ud)
{
	int idx = 0;
	for (int i = 0; i < m_mesh_data->vert_num; ++i)
	{
		sm::vec2 new_xy(0, 0);
		Skin2Vertex& vertex = m_mesh_data->vertices[i];
		for (int j = 0, m = m_mesh_data->vertices_joint[i]; j < m; ++j) 
		{
			const Skin2Joint& joint = m_mesh_data->joints[idx++];
			const float* mat = query_joint_world_mt(joint.joint, ud);
			sm::vec2 v = joint.vertex + joint.offset;
			new_xy.x += (v.x * mat[0] + v.y * mat[2] + mat[4]) * joint.weight;
			new_xy.y += (v.x * mat[1] + v.y * mat[3] + mat[5]) * joint.weight;
		}
		vertex.xy = new_xy;
	}
}

void Skin2Mesh::Update(int offset0, int count0, 
						   int offset1, int count1, 
						   const float* vertices)
{
 	for (int i = 0; i < count0; ++i) {
 		m_mesh_data->joints[offset0 + i].offset.Set(0, 0);
 	}
 	for (int i = 0; i < count1; ++i) {
 		m_mesh_data->joints[offset1 + i].offset.Set(0, 0);
 	}
 
 	int ptr = 0;
 	for (int i = 0; i < count0; ++i) {
 		sm::vec2 offset(vertices[ptr++], vertices[ptr++]);
 		m_mesh_data->joints[offset0 + i].offset += offset;
 	}
 	for (int i = 0; i < count1; ++i) {
 		sm::vec2 offset(vertices[ptr++], vertices[ptr++]);
 		m_mesh_data->joints[offset1 + i].offset += offset;
 	}
}

}