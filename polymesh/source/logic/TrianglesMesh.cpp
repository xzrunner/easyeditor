#include "TrianglesMesh.h"

namespace pm
{

TrianglesMesh::TrianglesMesh(const std::vector<sm::vec2>& vertices, 
							 const std::vector<sm::vec2>& texcoords, 
							 const std::vector<int>& triangles)
{
	m_mesh_data = Triangles::Create(vertices, texcoords, triangles);
}

TrianglesMesh::~TrianglesMesh()
{
	delete m_mesh_data;
}

void TrianglesMesh::Update(int offset0, int count0, 
						   int offset1, int count1, 
						   const float* vertices)
{
	for (int i = 0; i < count0; ++i) {
		m_mesh_data->m_vertices[offset0 + i].xy = m_mesh_data->m_vertices[offset0 + i].ori_xy;
	}
	for (int i = 0; i < count1; ++i) {
		m_mesh_data->m_vertices[offset1 + i].xy = m_mesh_data->m_vertices[offset1 + i].ori_xy;
	}

	int ptr = 0;
	for (int i = 0; i < count0; ++i) {
		m_mesh_data->m_vertices[offset0 + i].xy.x += vertices[ptr++];
		m_mesh_data->m_vertices[offset0 + i].xy.y += vertices[ptr++];
	}
	for (int i = 0; i < count1; ++i) {
		m_mesh_data->m_vertices[offset1 + i].xy.x += vertices[ptr++];
		m_mesh_data->m_vertices[offset1 + i].xy.y += vertices[ptr++];
	}
}

}