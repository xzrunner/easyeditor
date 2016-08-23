#include "Mesh.h"
#include "MeshTriangle.h"
#include "S2_Symbol.h"

namespace s2
{

Mesh::Mesh()
	: m_base(NULL)
	, m_width(0)
	, m_height(0)
	, m_node_radius(5)
{	
}

Mesh::Mesh(const Mesh& mesh)
	: m_width(mesh.m_width)
	, m_height(mesh.m_height)
	, m_node_radius(mesh.m_node_radius)
{
	mesh.m_base->AddReference();
	m_base = mesh.m_base;
}

Mesh::Mesh(const Symbol* base)
{
	base->AddReference();
	m_base = base;

	sm::vec2 sz = m_base->GetBounding().Size();
	m_width = sz.x;
	m_height = sz.y;

	m_node_radius = std::min(m_width * 0.1f, 5.0f);
}

Mesh::~Mesh()
{
	if (m_base) {
		m_base->RemoveReference();
	}

	ClearTriangles();
}

sm::rect Mesh::GetRegion() const
{
	sm::rect r;
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		MeshTriangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i) {
			r.Combine(tri->nodes[i]->xy);
		}
	}	
	return r;
}

void Mesh::ClearTriangles()
{
	for (int i = 0, n = m_tris.size(); i < n; ++i) {
		m_tris[i]->RemoveReference();
	}
	m_tris.clear();
}

}