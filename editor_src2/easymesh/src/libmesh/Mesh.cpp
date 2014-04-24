#include "Mesh.h"
#include "Triangle.h"

namespace emesh
{

Mesh::Mesh()
	: m_texid(0)
	, m_width(0)
	, m_height(0)
{
}

Mesh::Mesh(const Mesh& mesh)
	: m_texid(mesh.m_texid)
	, m_width(mesh.m_width)
	, m_height(mesh.m_height)
	, m_region(mesh.m_region)
{
	loadTriangles();
	copyTriangles(mesh);
}

Mesh::Mesh(const d2d::Image& image, bool initBound)
{
	m_texid = image.textureID();

	m_width = image.getRegion().xLength();
	m_height = image.getRegion().yLength();

	if (initBound)
	{
		float hw = m_width * 0.5f,
			hh = m_height * 0.5f;
		m_region.bound.reserve(4);
		m_region.bound.push_back(d2d::Vector(-hw,-hh));
		m_region.bound.push_back(d2d::Vector( hw,-hh));
		m_region.bound.push_back(d2d::Vector( hw, hh));
		m_region.bound.push_back(d2d::Vector(-hw, hh));
		loadTriangles();
	}
}

Mesh::~Mesh()
{
	clearTriangles();
}

Mesh* Mesh::clone() const
{
	return new Mesh(*this);
}

void Mesh::insert(const d2d::Vector& p)
{
	if (d2d::Math::isPointInArea(p, m_region.bound))
	{
		m_region.nodes.push_back(p);
		loadTriangles();
	}
}

void Mesh::remove(const d2d::Vector& p)
{
	if (d2d::Math::isPointInArea(p, m_region.bound))
	{
		std::vector<d2d::Vector>::iterator itr
			= m_region.nodes.begin();
		for ( ; itr != m_region.nodes.end(); ++itr)
		{
			if (d2d::Math::getDistance(*itr, p) < Node::RADIUS)
			{
				m_region.nodes.erase(itr);
				loadTriangles();
				break;
			}
		}
	}
}

d2d::Vector* Mesh::find(const d2d::Vector& p)
{
	for (int i = 0, m = m_region.bound.size(); i < m; ++i)
		if (d2d::Math::getDistance(m_region.bound[i], p) < Node::RADIUS)
			return &m_region.bound[i];
	for (int i = 0, m = m_region.nodes.size(); i < m; ++i)
		if (d2d::Math::getDistance(m_region.nodes[i], p) < Node::RADIUS)
			return &m_region.nodes[i];
	return NULL;
}

void Mesh::move(d2d::Vector* src, const d2d::Vector& dst)
{
	if (!src) return;

	if (d2d::Math::isPointInArea(dst, m_region.bound))
	{
		d2d::Vector d = dst;
		fixNodeToRegion(d);
		*src = d;
		loadTriangles();
	}
}

Node* Mesh::queryNode(const d2d::Vector& p)
{
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int j = 0; j < 3; ++j)
		{
			if (d2d::Math::getDistance(tri->nodes[j]->xy, p) < Node::RADIUS)
				return tri->nodes[j];
		}
	}
	return NULL;
}

void Mesh::queryNode(const d2d::Rect& r, std::vector<Node*>& nodes)
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

void Mesh::drawInfoUV() const
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

void Mesh::drawInfoXY() const
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

void Mesh::drawTexture() const
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

void Mesh::tween(const Mesh& begin, const Mesh& end, float process)
{
	assert(m_tris.size() == begin.m_tris.size() && m_tris.size() == end.m_tris.size());
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int j = 0; j < 3; ++j)
		{
			const d2d::Vector& p0 = begin.m_tris[i]->nodes[j]->xy;
			const d2d::Vector& p1 = end.m_tris[i]->nodes[j]->xy;
			m_tris[i]->nodes[j]->xy = p0 + (p1 - p0) * process;
		}
	}
}

void Mesh::reset()
{
	loadTriangles();
}

void Mesh::clear()
{
	m_region.nodes.clear();
	loadTriangles();
}

void Mesh::loadTriangles()
{
	clearTriangles();

	std::vector<d2d::Vector> result;
	std::map<d2d::Vector, Node*, d2d::VectorCmp> map2Node;
	Node null;
	d2d::Triangulation::points(m_region.bound, m_region.nodes, result);
	for (int i = 0, n = result.size(); i < n; ++i)
		map2Node.insert(std::make_pair(result[i], &null));

	for (int i = 0, n = result.size() / 3, ptr = 0; i < n; ++i)
	{
		Triangle* tri = new Triangle;
		for (int j = 0; j < 3; ++j)
		{
			std::map<d2d::Vector, Node*, d2d::VectorCmp>::iterator itr 
				= map2Node.find(result[ptr++]);
			assert(itr != map2Node.end());
			if (itr->second == &null)
				itr->second = new Node(itr->first, m_width, m_height);
			tri->nodes[j] = itr->second;
		}
		m_tris.push_back(tri);
	}
}

void Mesh::clearTriangles()
{
	for_each(m_tris.begin(), m_tris.end(), DeletePointerFunctor<Triangle>());
	m_tris.clear();
}

void Mesh::copyTriangles(const Mesh& mesh)
{
	assert(m_tris.size() == mesh.m_tris.size());
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* src = mesh.m_tris[i];
		Triangle* dst = m_tris[i];
		for (int j = 0; j < 3; ++j)
		{
			dst->nodes[j]->xy = src->nodes[j]->xy;
		}
	}
}

void Mesh::fixNodeToRegion(d2d::Vector& node)
{
	const float REGION = 5;

	for (int i = 0, n = m_region.bound.size(); i < n; ++i)
	{
		const d2d::Vector& b = m_region.bound[i];
		if (fabs(node.x - b.x) < REGION) {
			node.x = b.x;
		}
		if (fabs(node.y - b.y) < REGION) {
			node.y = b.y;
		}
	}
}

}