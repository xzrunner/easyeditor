#include "Shape.h"
#include "Triangle.h"

#include <drag2d.h>

namespace emesh
{

Shape::Shape()
	: m_texid(0)
	, m_width(0)
	, m_height(0)
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
			if (d2d::Math::getDistance(tri->nodes[j]->xy, p) < Node::RADIUS) {
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
	d2d::PrimitiveDraw::drawCircles(nodes, Node::RADIUS, true, 2, d2d::Colorf(0.4f, 0.2f, 0.8f, 0.5f));
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
	d2d::PrimitiveDraw::drawCircles(nodes, Node::RADIUS, true, 2, d2d::Colorf(0.4f, 0.2f, 0.8f, 0.5f));
}

void Shape::DrawTexture() const
{
	std::vector<d2d::Vector> vertices, texcoords;
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i)
		{
			texcoords.push_back(tri->nodes[i]->uv);
			vertices.push_back(tri->nodes[i]->xy);
		}
	}

	d2d::PrimitiveDraw::drawTriangles(m_texid, vertices, texcoords);
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