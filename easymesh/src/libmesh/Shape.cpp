#include "Shape.h"
#include "Triangle.h"

#include <drag2d.h>

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
	, m_width(shape.m_width)
	, m_height(shape.m_height)
{
}

Shape::Shape(const d2d::Image& image)
{
	m_texid = image.textureID();

	m_width = image.getRegion().xLength();
	m_height = image.getRegion().yLength();

	m_node_radius = std::min(m_width * 0.1f, 5.0f);
}

Shape::~Shape()
{
	ClearTriangles();
}

void Shape::QueryNode(const d2d::Vector& p, std::vector<Node*>& nodes)
{
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int j = 0; j < 3; ++j) {
			if (d2d::Math::getDistance(tri->nodes[j]->xy, p) < m_node_radius) {
				nodes.push_back(tri->nodes[j]);
			}
		}
	}
}

void Shape::QueryNode(const d2d::Rect& r, std::vector<Node*>& nodes)
{
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int j = 0; j < 3; ++j)
		{
			if (d2d::Math::isPointInRect(tri->nodes[j]->xy, r))
				nodes.push_back(tri->nodes[j]);
		}
	}
}

void Shape::DrawInfoUV() const
{
	std::set<d2d::Vector, d2d::VectorCmp> unique;
	std::vector<d2d::Vector> tmp(3);
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i)
		{
			tmp[i].x = (tri->nodes[i]->uv.x - 0.5f) * m_width;
			tmp[i].y = (tri->nodes[i]->uv.y - 0.5f) * m_height;
			unique.insert(tmp[i]);
		}
		d2d::PrimitiveDraw::drawPolyline(tmp, d2d::Colorf(0.8f, 0.2f, 0.4f, 0.5f), true);
	}
	std::vector<d2d::Vector> nodes;
	copy(unique.begin(), unique.end(), back_inserter(nodes));
	d2d::PrimitiveDraw::drawCircles(nodes, m_node_radius, true, 2, d2d::Colorf(0.4f, 0.2f, 0.8f, 0.5f));
}

void Shape::DrawInfoXY() const
{
	std::set<d2d::Vector, d2d::VectorCmp> unique;
	std::vector<d2d::Vector> tmp(3);
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i)
		{
			tmp[i] = tri->nodes[i]->xy;
			unique.insert(tmp[i]);
		}
		d2d::PrimitiveDraw::drawPolyline(tmp, d2d::Colorf(0.8f, 0.2f, 0.4f, 0.5f), true);
	}
	std::vector<d2d::Vector> nodes;
	copy(unique.begin(), unique.end(), back_inserter(nodes));
	d2d::PrimitiveDraw::drawCircles(nodes, m_node_radius, true, 2, d2d::Colorf(0.4f, 0.2f, 0.8f, 0.5f));
}

void Shape::DrawTexture(const d2d::Matrix& mt) const
{
	d2d::ShaderNew* shader = d2d::ShaderNew::Instance();
	shader->sprite();
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		d2d::Vector vertices[4], texcoords[4];
		for (int i = 0; i < 3; ++i)
		{
			vertices[i] = d2d::Math::transVector(tri->nodes[i]->xy, mt);
			texcoords[i] = tri->nodes[i]->uv;
		}
		vertices[3] = vertices[2];
		texcoords[3] = vertices[2];
		shader->Draw(vertices, texcoords, m_texid);
	}
}

d2d::Rect Shape::GetRegion() const
{
	d2d::Rect r;
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i) {
			r.combine(tri->nodes[i]->xy);
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
			const d2d::Vector& p0 = begin->m_tris[i]->nodes[j]->xy;
			const d2d::Vector& p1 = end->m_tris[i]->nodes[j]->xy;
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
	std::vector<d2d::Vector> transform;
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i)
			transform.push_back(tri->nodes[i]->xy);
	}
	d2d::JsonTools::store(transform, value);
}

void Shape::LoadTriangles(const Json::Value& value)
{
	std::vector<d2d::Vector> transform;
	d2d::JsonTools::load(value, transform);
	int itr = 0;
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i)
			tri->nodes[i]->xy = transform[itr++];
	}
}

}