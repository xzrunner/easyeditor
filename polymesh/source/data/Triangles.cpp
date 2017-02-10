#include "Triangles.h"
#include "MeshTransform.h"
#include "pm_define.h"

#include <assert.h>

namespace pm
{

Triangles::Triangles() 
	: m_vert_num(0)
	, m_tri_num(0)
	, m_triangles(NULL)
{
}

//void Triangles::Reset()
//{
//	for (int i = 0; i < vert_num; ++i) {
//		vertices[i].xy = vertices[i].ori_xy;
//	}
//}

void Triangles::Dump(std::vector<sm::vec2>& out_vertices, 
					 std::vector<sm::vec2>& out_texcoords, 
					 std::vector<int>& out_triangles) const
{
	out_vertices.resize(m_vert_num);
	out_texcoords.resize(m_vert_num);
	for (int i = 0; i < m_vert_num; ++i) {
		out_vertices[i]  = m_vertices[i].xy;
		out_texcoords[i] = m_vertices[i].uv;
	}

	out_triangles.resize(m_tri_num);
	for (int i = 0; i < m_tri_num; ++i) {
		out_triangles[i] = m_triangles[i];
	}
}

void Triangles::LoadFromTransform(const MeshTransform& transform)
{
	const std::vector<std::pair<int, sm::vec2> >& trans = transform.GetTrans();
	for (int i = 0, n = trans.size(); i < n; ++i) {
		m_vertices[trans[i].first].xy = m_vertices[trans[i].first].ori_xy;
	}
	for (int i = 0, n = trans.size(); i < n; ++i) {
		m_vertices[trans[i].first].xy += trans[i].second;
	}
}

void Triangles::StoreToTransform(MeshTransform& transform) const
{
	std::vector<std::pair<int, sm::vec2> > trans;
	for (int i = 0; i < m_vert_num; ++i) {
		const Vertex& v = m_vertices[i];
		if (v.xy != v.ori_xy) {
			trans.push_back(std::make_pair(i, v.xy - v.ori_xy));
		}
	}
	transform.SetTrans(trans);
}

Triangles* Triangles::Create(const std::vector<sm::vec2>& vertices, 
							 const std::vector<sm::vec2>& texcoords, 
							 const std::vector<int>& triangles)
{
	assert(vertices.size() == texcoords.size());
	int head_sz = sizeof(Triangles) + PTR_SIZE_DIFF - sizeof(Vertex);
	int sz = head_sz;
	sz += sizeof(Vertex) * vertices.size();			// vertices
	sz += sizeof(uint16_t) * triangles.size();		// triangles

	uint8_t* ptr = new uint8_t[sz];
	Triangles* ret = new (ptr) Triangles();
	ret->m_vert_num = vertices.size();
	ret->m_tri_num = triangles.size();
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		ret->m_vertices[i].xy = ret->m_vertices[i].ori_xy = vertices[i];
		ret->m_vertices[i].uv = texcoords[i];
	}
	ret->m_triangles = (uint16_t*)(ptr + (head_sz + sizeof(Vertex) * vertices.size()));
	for (int i = 0, n = triangles.size(); i < n; ++i) {
		ret->m_triangles[i] = triangles[i];
	}

	return ret;
}

}