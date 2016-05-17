#include "Mesh.h"

#include <ee/Triangulation.h>
#include <ee/EE_RVG.h>

namespace enav
{

Mesh::Mesh(const sm::rect& region)
{
	m_bound.push_back(sm::vec2(region.xmin, region.ymin));
	m_bound.push_back(sm::vec2(region.xmin, region.ymax));
	m_bound.push_back(sm::vec2(region.xmax, region.ymax));
	m_bound.push_back(sm::vec2(region.xmax, region.ymin));
}

void Mesh::AddHole(const std::vector<sm::vec2>& hole)
{
	m_holes.push_back(Hole(hole));

	Build();
}

void Mesh::Build()
{
	std::vector<std::vector<sm::vec2> > holes;
	for (int i = 0, n = m_holes.size(); i < n; ++i) {
		holes.push_back(m_holes[i].edge);
	}
	ee::Triangulation::Holes(m_bound, holes, m_triangles);
}

void Mesh::Draw() const
{
// 	for (int i = 0, n = m_triangles.size(); i < n; i += 3) {
// 		std::vector<sm::vec2> tri;
// 		tri.push_back(m_triangles[i]);
// 		tri.push_back(m_triangles[i+1]);
// 		tri.push_back(m_triangles[i+2]);
// 		ee::RVG::Triangles()
// 	}

	ee::RVG::Triangles(m_triangles);
}

}