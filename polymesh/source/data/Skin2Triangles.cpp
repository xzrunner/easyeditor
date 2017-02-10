#include "Skin2Triangles.h"
#include "MeshTransform.h"
#include "pm_define.h"

#include <assert.h>

namespace pm
{

Skin2Triangles::Skin2Triangles()
	: m_part_num(0)
	, m_vert_num(0)
	, m_tri_num(0)
	, m_vertices(NULL)
	, m_triangles(NULL)
{
}

void Skin2Triangles::Dump(std::vector<sm::vec2>& out_vertices, 
						  std::vector<sm::vec2>& out_texcoords, 
						  std::vector<int>& out_triangles) const
{
	out_vertices.resize(m_vert_num);
	out_texcoords.resize(m_vert_num);
	for (int i = 0; i < m_vert_num; ++i) {
		const Skin2Vertex* v = m_vertices[i];
		out_vertices[i]  = v->xy;
		out_texcoords[i] = v->uv;
	}

	out_triangles.resize(m_tri_num);
	for (int i = 0; i < m_tri_num; ++i) {
		out_triangles[i] = m_triangles[i];
	}
}

void Skin2Triangles::LoadFromTransform(const MeshTransform& transform)
{
	const std::vector<std::pair<int, sm::vec2> >& trans = transform.GetTrans();
	for (int i = 0, n = trans.size(); i < n; ++i) {
		const Skin2Vertex* v = m_vertices[trans[i].first];
		const sm::vec2& offset = trans[i].second;
		for (int j = 0; j < v->num; ++j) {
			Skin2Vertex::Part& p = m_parts[v->parts[j]];
			p.offset = offset * p.weight;
		}
	}
}

void Skin2Triangles::StoreToTransform(MeshTransform& transform) const
{
	std::vector<std::pair<int, sm::vec2> > trans;
	for (int i = 0; i < m_vert_num; ++i)
	{
		const Skin2Vertex* v = m_vertices[i];
		sm::vec2 offset(0, 0);
		for (int j = 0; j < v->num; ++j) {
			const Skin2Vertex::Part& p = m_parts[v->parts[j]];
			offset += p.offset * p.weight;
		}
		if (offset != sm::vec2(0, 0)) {
			trans.push_back(std::make_pair(i, offset));
		}
	}
}

Skin2Triangles* Skin2Triangles::Create(const std::vector<Skin2Vertex::Part>& parts,
									   const std::vector<int>& vertices,
									   const std::vector<sm::vec2>& texcoords,
									   const std::vector<int>& triangles)
{
	assert(vertices.size() == texcoords.size());
	int head_sz = sizeof(Skin2Triangles) + PTR_SIZE_DIFF * 2 - sizeof(Skin2Vertex::Part);
	int sz = head_sz;
	// parts
	sz += sizeof(Skin2Vertex::Part) * parts.size();
	// vertices
	sz += sizeof(Skin2Vertex**) * vertices.size();
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		sz += sizeof(Skin2Vertex) - sizeof(uint16_t) + sizeof(uint16_t) * vertices[i];
	}
	// triangles
	sz += sizeof(uint16_t) * triangles.size();

	uint8_t* ptr = new uint8_t[sz];
	Skin2Triangles* ret = new (ptr) Skin2Triangles();
	ret->m_part_num = parts.size();
	ret->m_vert_num = vertices.size();
	ret->m_tri_num  = triangles.size();
	// parts
	for (int i = 0, n = parts.size(); i < n; ++i) {
		ret->m_parts[i] = parts[i];
	}
	// vertices
	ptr += head_sz + sizeof(Skin2Vertex::Part) * parts.size();
	ret->m_vertices = reinterpret_cast<Skin2Vertex**>(ptr);
	ptr += sizeof(Skin2Vertex**) * vertices.size();
	int part_ptr = 0;
	for (int i = 0, n = vertices.size(); i < n; ++i) 
	{
		Skin2Vertex* v = reinterpret_cast<Skin2Vertex*>(ptr);
		v->uv = texcoords[i];
		v->num = vertices[i];
		for (int j = 0; j < v->num; ++j) {
			v->parts[j] = vertices[i][j];
		}
		ret->m_vertices[i] = v;

		ptr += sizeof(Skin2Vertex) - sizeof(uint16_t) + sizeof(uint16_t) * vertices[i];
	}
	// triangles
	ret->m_triangles = reinterpret_cast<uint16_t*>(ptr);
	for (int i = 0, n = triangles.size(); i < n; ++i) {
		ret->m_triangles[i] = triangles[i];
	}

	return ret;
}

}