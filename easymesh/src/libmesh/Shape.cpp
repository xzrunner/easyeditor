#include "Shape.h"
#include "Triangle.h"

namespace emesh
{

Shape::Shape()
	: m_texid(0)
	, m_width(0)
	, m_height(0)
	, m_node_radius(5)
{
}

Shape::Shape(const Shape& shape)
	: m_texid(shape.m_texid)
	, m_tex_filepath(shape.m_tex_filepath)
	, m_width(shape.m_width)
	, m_height(shape.m_height)
{
}

Shape::Shape(const ee::Image& image)
{
	m_texid = image.GetTexID();
	m_tex_filepath = image.GetFilepath();

	m_width = image.GetOriginWidth();
	m_height = image.GetOriginHeight();

	m_node_radius = std::min(m_width * 0.1f, 5.0f);
}

Shape::~Shape()
{
	ClearTriangles();
}

void Shape::QueryNode(const ee::Vector& p, std::vector<Node*>& nodes)
{
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int j = 0; j < 3; ++j) {
			if (ee::Math2D::GetDistance(tri->nodes[j]->xy, p) < m_node_radius) {
				nodes.push_back(tri->nodes[j]);
			}
		}
	}
}

void Shape::QueryNode(const ee::Rect& r, std::vector<Node*>& nodes)
{
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int j = 0; j < 3; ++j)
		{
			if (ee::Math2D::IsPointInRect(tri->nodes[j]->xy, r))
				nodes.push_back(tri->nodes[j]);
		}
	}
}

void Shape::DrawInfoUV() const
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
		ee::PrimitiveDraw::DrawPolyline(tmp, ee::Colorf(0.8f, 0.2f, 0.4f, 0.5f), true);
	}
	std::vector<ee::Vector> nodes;
	copy(unique.begin(), unique.end(), back_inserter(nodes));
	ee::PrimitiveDraw::DrawCircles(nodes, m_node_radius, true, 2, ee::Colorf(0.4f, 0.2f, 0.8f, 0.5f));
}

void Shape::DrawInfoXY() const
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
		ee::PrimitiveDraw::DrawPolyline(tmp, ee::Colorf(0.8f, 0.2f, 0.4f, 0.5f), true);
	}
	std::vector<ee::Vector> nodes;
	copy(unique.begin(), unique.end(), back_inserter(nodes));
	ee::PrimitiveDraw::DrawCircles(nodes, m_node_radius, true, 2, ee::Colorf(0.4f, 0.2f, 0.8f, 0.5f));
}

void Shape::DrawTexture(const ee::Matrix& mt,
						const ee::Colorf& mul, 
						const ee::Colorf& add) const
{
	DrawTexture(mt, mul, add, m_texid);
}

void Shape::DrawTexture(const ee::Matrix& mt, 
						const ee::Colorf& mul, 
						const ee::Colorf& add,
						unsigned int texid) const
{
	ee::ShaderMgr* shader = ee::ShaderMgr::Instance();
	shader->sprite();
	ee::ColorTrans color;
	color.multi = mul;
	color.add = add;
	shader->SetSpriteColor(color);
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		ee::Vector vertices[4], texcoords[4];
		for (int i = 0; i < 3; ++i)
		{
			vertices[i] = ee::Math2D::TransVector(tri->nodes[i]->xy, mt);
			texcoords[i] = tri->nodes[i]->uv;
		}
		vertices[3] = vertices[2];
		texcoords[3] = texcoords[2];

// 		if (ee::Config::Instance()->IsUseDTex()) {
// 			ee::DynamicTexAndFont::Instance()->Draw(vertices, texcoords, 
// 				m_tex_filepath, m_texid);
// 		} else {
			shader->Draw(vertices, texcoords, m_texid);
//		}
	}
}

ee::Rect Shape::GetRegion() const
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

void Shape::SetTween(Shape* begin, Shape* end, float process)
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

void Shape::ClearTriangles()
{
	for_each(m_tris.begin(), m_tris.end(), DeletePointerFunctor<Triangle>());
	m_tris.clear();
}

void Shape::StoreTriangles(Json::Value& value) const
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

void Shape::LoadTriangles(const Json::Value& value)
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