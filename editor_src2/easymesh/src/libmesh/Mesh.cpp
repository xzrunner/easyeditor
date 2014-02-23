#include "Mesh.h"
#include "Triangle.h"

namespace emesh
{

Mesh::Mesh()
	: m_width(0)
	, m_height(0)
{
}

Mesh::Mesh(const Mesh& mesh)
: m_width(mesh.m_width)
, m_height(mesh.m_height)
, m_regions(mesh.m_regions)
{
	m_tris.reserve(mesh.m_tris.size());
	for (size_t i = 0, n = mesh.m_tris.size(); i < n; ++i)
		m_tris.push_back(new Triangle(*mesh.m_tris[i])); 
}

Mesh::Mesh(float width, float height)
	: m_width(width)
	, m_height(height)
{
}

Mesh::~Mesh()
{
	clearTriangles();
}

void Mesh::insert(const d2d::Vector& p)
{
	
}

void Mesh::remove(const d2d::Vector& p)
{
	
}

void Mesh::loadTriangles()
{
	clearTriangles();
	for (int i = 0, n = m_regions.size(); i < n; ++i)
	{
		const Region& r = m_regions[i];
		std::vector<d2d::Vector> result;
		d2d::Triangulation::points(r.bound, r.nodes, result);
		for (int i = 0, n = result.size() / 3, ptr = 0; i < n; ++i)
		{
			Triangle* tri = new Triangle(m_width, m_height, 
				result[ptr++], result[ptr++], result[ptr++]);
			m_tris.push_back(tri);
		}
	}
}

void Mesh::clearTriangles()
{
	for_each(m_tris.begin(), m_tris.end(), DeletePointerFunctor<Triangle>());
	m_tris.clear();
}

}