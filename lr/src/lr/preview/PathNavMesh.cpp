#include "PathNavMesh.h"

#include <easyshape.h>

namespace lr
{
namespace preview
{

PathNavMesh::PathNavMesh(const d2d::Rect& region)
	: m_region(region)
	, m_visited(this)
{
}

void PathNavMesh::DisableRegion(const d2d::Sprite* spr, bool disable)
{
	m_tris.clear();

	// get bound
	std::vector<d2d::Vector> bound;
	const libshape::Sprite* shape = dynamic_cast<const libshape::Sprite*>(spr);
	if (shape && shape->GetSymbol().GetShapeType() == libshape::ST_POLYGON)
	{
		const std::vector<d2d::Shape*>& shapes = shape->GetSymbol().GetShapes();
		const libshape::PolygonShape* poly = static_cast<const libshape::PolygonShape*>(shapes[0]);
		bound = poly->GetVertices();
	}
	else
	{
		spr->GetBounding()->GetBoundPos(bound);
	}

	std::map<const d2d::Sprite*, std::vector<d2d::Vector> >::iterator itr = m_bounds.find(spr);
	if (itr == m_bounds.end() && !disable) {
		std::vector<d2d::Vector> fixed;
		d2d::Math2D::RemoveDuplicatePoints(bound, fixed);
		d2d::Matrix mat;
		spr->GetTransMatrix(mat);
		for (int i = 0; i < fixed.size(); ++i) {
			fixed[i] = d2d::Math2D::TransVector(fixed[i], mat);
		}
		m_bounds.insert(std::make_pair(spr, fixed));
	} else if (itr != m_bounds.end() && disable) {
		m_bounds.erase(itr);
	}

	// create nodes
	std::vector<d2d::Vector> all_bound;
	all_bound.push_back(d2d::Vector(m_region.xmin, m_region.ymin));
	all_bound.push_back(d2d::Vector(m_region.xmin, m_region.ymax));
	all_bound.push_back(d2d::Vector(m_region.xmax, m_region.ymax));
	all_bound.push_back(d2d::Vector(m_region.xmax, m_region.ymin));

	std::vector<d2d::Vector> lines;
	std::vector<std::vector<d2d::Vector> > loops;
	for (itr = m_bounds.begin(); itr != m_bounds.end(); ++itr) {
		loops.push_back(itr->second);
	}
	d2d::Triangulation::LinesAndLoops(all_bound, lines, loops, m_tris);
}

void PathNavMesh::QueryRoute(const d2d::Vector& start, const d2d::Vector& end)
{
	
}

void PathNavMesh::DebugDraw() const
{
	d2d::PrimitiveDraw::DrawTrianglesLine(m_tris, d2d::LIGHT_RED);
}

d2d::Vector PathNavMesh::TransIDToPos(int id) const
{
	d2d::Vector ret;
	ret.SetInvalid();

	return ret;
}

}
}