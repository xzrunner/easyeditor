#include "Mesh.h"
#include "Triangle.h"

#include <ee/Image.h>
#include <ee/EE_RVG.h>
#include <ee/Math2D.h>
#include <ee/RenderColor.h>
#include <ee/std_functor.h>
#include <ee/JsonSerializer.h>
#include <ee/RenderParams.h>
#include <ee/trans_color.h>

#include <shaderlab.h>

#include <set>
#include <algorithm>

#include <assert.h>

namespace emesh
{

Mesh::Mesh()
	: m_texid(0)
	, m_width(0)
	, m_height(0)
	, m_node_radius(5)
{
}

Mesh::Mesh(const Mesh& mesh)
	: m_texid(mesh.m_texid)
	, m_tex_filepath(mesh.m_tex_filepath)
	, m_width(mesh.m_width)
	, m_height(mesh.m_height)
	, m_node_radius(mesh.m_node_radius)
{
}

Mesh::Mesh(const ee::Image& image)
{
	m_texid = image.GetTexID();
	m_tex_filepath = image.GetFilepath();

	m_width  = static_cast<float>(image.GetOriginWidth());
	m_height = static_cast<float>(image.GetOriginHeight());

	m_node_radius = std::min(m_width * 0.1f, 5.0f);
}

Mesh::~Mesh()
{
	ClearTriangles();
}

Node* Mesh::PointQueryNode(const ee::Vector& p)
{
	Node* node = NULL;
	float nearest = FLT_MAX;
	for (int i = 0, n = m_tris.size(); i < n; ++i) {
		Triangle* tri = m_tris[i];
		for (int j = 0; j < 3; ++j) {
			float dis = ee::Math2D::GetDistance(tri->nodes[j]->xy, p);
			if (dis < m_node_radius && dis < nearest) {
				nearest = dis;
				node = tri->nodes[j];
			}
		}
	}

	return node;
}

void Mesh::RectQueryNodes(const ee::Rect& r, std::vector<Node*>& nodes)
{
	std::set<Node*> unique;
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int j = 0; j < 3; ++j) {
			Node* node = tri->nodes[j];
			if (ee::Math2D::IsPointInRect(node->xy, r) &&
				unique.find(node) == unique.end()) {
				nodes.push_back(node);
				unique.insert(node);
			}
		}
	}
}

void Mesh::DrawInfoUV() const
{
	std::set<ee::Vector, ee::VectorCmp> unique;
	std::vector<ee::Vector> tmp(3);
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i)
		{
			tmp[i].x = (tri->nodes[i]->uv.x - 0.5f) * m_width;
			tmp[i].y = (tri->nodes[i]->uv.y - 0.5f) * m_height;
			unique.insert(tmp[i]);
		}
		ee::RVG::Color(ee::Colorf(0.8f, 0.2f, 0.4f, 0.5f));
		ee::RVG::Polyline(tmp, true);
	}
	std::vector<ee::Vector> nodes;
	copy(unique.begin(), unique.end(), back_inserter(nodes));
	ee::RVG::Color(ee::Colorf(0.4f, 0.2f, 0.8f, 0.5f));
	ee::RVG::Circles(nodes, m_node_radius, true);
}

void Mesh::DrawInfoXY() const
{
	std::set<ee::Vector, ee::VectorCmp> unique;
	std::vector<ee::Vector> tmp(3);
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i)
		{
			tmp[i] = tri->nodes[i]->xy;
			unique.insert(tmp[i]);
		}
		ee::RVG::Color(ee::Colorf(0.8f, 0.2f, 0.4f, 0.5f));
		ee::RVG::Polyline(tmp, true);
	}
	std::vector<ee::Vector> nodes;
	copy(unique.begin(), unique.end(), back_inserter(nodes));
	ee::RVG::Color(ee::Colorf(0.4f, 0.2f, 0.8f, 0.5f));
	ee::RVG::Circles(nodes, m_node_radius, true);
}

void Mesh::DrawTexture(const ee::RenderParams& trans) const
{
	DrawTexture(trans, m_texid);
}

void Mesh::DrawTexture(const ee::RenderParams& trans,
						unsigned int texid) const
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader());
	shader->SetColor(ee::color2int(trans.color.multi, ee::PT_ABGR),
		ee::color2int(trans.color.add, ee::PT_ABGR));
	shader->SetColorMap(ee::color2int(trans.color.r, ee::PT_ABGR),
		ee::color2int(trans.color.g, ee::PT_ABGR),
		ee::color2int(trans.color.b, ee::PT_ABGR));
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		ee::Vector vertices[4], texcoords[4];
		for (int i = 0; i < 3; ++i)
		{
			vertices[i] = ee::Math2D::TransVector(tri->nodes[i]->xy, trans.mt);
			texcoords[i] = tri->nodes[i]->uv;
		}
		vertices[3] = vertices[2];
		texcoords[3] = texcoords[2];

// 		if (ee::Config::Instance()->IsUseDTex()) {
// 			ee::DynamicTexAndFont::Instance()->Draw(vertices, texcoords, 
// 				m_tex_filepath, m_texid);
// 		} else {
			shader->Draw(&vertices[0].x, &texcoords[0].x, m_texid);
//		}
	}
}

ee::Rect Mesh::GetRegion() const
{
	ee::Rect r;
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i) {
			r.Combine(tri->nodes[i]->xy);
		}
	}	
	return r;
}

void Mesh::SetTween(Mesh* begin, Mesh* end, float process)
{
	assert(m_tris.size() == begin->m_tris.size() && m_tris.size() == end->m_tris.size());
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int j = 0; j < 3; ++j)
		{
			const ee::Vector& p0 = begin->m_tris[i]->nodes[j]->xy;
			const ee::Vector& p1 = end->m_tris[i]->nodes[j]->xy;
			m_tris[i]->nodes[j]->xy = p0 + (p1 - p0) * process;
		}
	}
}

void Mesh::ClearTriangles()
{
	for_each(m_tris.begin(), m_tris.end(), ee::DeletePointerFunctor<Triangle>());
	m_tris.clear();
}

void Mesh::StoreTriangles(Json::Value& value) const
{
	std::vector<ee::Vector> transform;
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i)
			transform.push_back(tri->nodes[i]->xy);
	}
	ee::JsonSerializer::Store(transform, value);
}

void Mesh::LoadTriangles(const Json::Value& value)
{
	std::vector<ee::Vector> transform;
	ee::JsonSerializer::Load(value, transform);
	int itr = 0;
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i)
			tri->nodes[i]->xy = transform[itr++];
	}
}

}