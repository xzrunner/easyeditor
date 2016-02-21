#include "PathNavMesh.h"

#include <ee/BoundingBox.h>
#include <ee/Math2D.h>
#include <ee/Matrix.h>
#include <ee/style_config.h>
#include <ee/PrimitiveDraw.h>
#include <ee/Triangulation.h>

#include <easyshape.h>

namespace lr
{
namespace preview
{

PathNavMesh::PathNavMesh(const ee::Rect& region)
	: m_region(region)
	, m_visited(this)
{
}

void PathNavMesh::DisableRegion(const ee::Sprite* spr, bool disable)
{
	m_tris.clear();

	// get bound
	std::vector<ee::Vector> bound;
	const eshape::Sprite* shape = dynamic_cast<const eshape::Sprite*>(spr);
	if (shape && shape->GetSymbol().GetShapeType() == eshape::ST_POLYGON)
	{
		const std::vector<ee::Shape*>& shapes = shape->GetSymbol().GetShapes();
		const eshape::PolygonShape* poly = static_cast<const eshape::PolygonShape*>(shapes[0]);
		bound = poly->GetVertices();
	}
	else
	{
		spr->GetBounding()->GetBoundPos(bound);
	}

	std::map<const ee::Sprite*, std::vector<ee::Vector> >::iterator itr = m_bounds.find(spr);
	if (itr == m_bounds.end() && !disable) {
		std::vector<ee::Vector> fixed;
		ee::Math2D::RemoveDuplicatePoints(bound, fixed);
		ee::Matrix mat;
		spr->GetTransMatrix(mat);
		for (int i = 0; i < fixed.size(); ++i) {
			fixed[i] = ee::Math2D::TransVector(fixed[i], mat);
		}
		m_bounds.insert(std::make_pair(spr, fixed));
	} else if (itr != m_bounds.end() && disable) {
		m_bounds.erase(itr);
	}

	// create nodes
	std::vector<ee::Vector> all_bound;
	all_bound.push_back(ee::Vector(m_region.xmin, m_region.ymin));
	all_bound.push_back(ee::Vector(m_region.xmin, m_region.ymax));
	all_bound.push_back(ee::Vector(m_region.xmax, m_region.ymax));
	all_bound.push_back(ee::Vector(m_region.xmax, m_region.ymin));

	std::vector<ee::Vector> lines;
	std::vector<std::vector<ee::Vector> > loops;
	for (itr = m_bounds.begin(); itr != m_bounds.end(); ++itr) {
		loops.push_back(itr->second);
	}
	ee::Triangulation::LinesAndLoops(all_bound, lines, loops, m_tris);
}

void PathNavMesh::QueryRoute(const ee::Vector& start, const ee::Vector& end)
{
	
}

void PathNavMesh::DebugDraw() const
{
	ee::PrimitiveDraw::DrawTrianglesLine(m_tris, ee::LIGHT_RED);
}

ee::Vector PathNavMesh::TransIDToPos(int id) const
{
	ee::Vector ret;
	ret.SetInvalid();

	return ret;
}

}
}