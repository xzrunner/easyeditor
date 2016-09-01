#include "PathNavMesh.h"

#include <ee/Math2D.h>
#include <ee/color_config.h>

#include <easyshape.h>

#include <sprite2/S2_RVG.h>
#include <sprite2/BoundingBox.h>
#include <SM_Triangulation.h>
#include <SM_Process.h>

namespace lr
{
namespace preview
{

PathNavMesh::PathNavMesh(const sm::rect& region)
	: m_region(region)
	, m_visited(this)
{
}

void PathNavMesh::DisableRegion(const ee::Sprite* spr, bool disable)
{
	m_tris.clear();

	// get bound
	std::vector<sm::vec2> bound;
	const eshape::Sprite* shape_spr = dynamic_cast<const eshape::Sprite*>(spr);
	if (shape_spr)
	{
		const eshape::Symbol* shape_sym = dynamic_cast<const eshape::Symbol*>(shape_spr->GetSymbol());
		if (shape_sym->GetShapeType() == eshape::ST_POLYGON) {
			const std::vector<ee::Shape*>& shapes = shape_sym->GetShapes();
			const eshape::PolygonShape* poly = static_cast<const eshape::PolygonShape*>(shapes[0]);
			bound = poly->GetVertices();
		}
	}
	else
	{
		sm::rect rect = spr->GetBounding()->GetSize();
		bound.push_back(sm::vec2(rect.xmin, rect.ymin));
		bound.push_back(sm::vec2(rect.xmax, rect.ymin));
		bound.push_back(sm::vec2(rect.xmax, rect.ymax));
		bound.push_back(sm::vec2(rect.xmin, rect.ymax));
	}

	std::map<const ee::Sprite*, std::vector<sm::vec2> >::iterator itr = m_bounds.find(spr);
	if (itr == m_bounds.end() && !disable) {
		std::vector<sm::vec2> fixed;
		sm::rm_duplicate_nodes(bound, fixed);
		sm::mat4 mat = spr->GetTransMatrix();
		for (int i = 0; i < fixed.size(); ++i) {
			fixed[i] = mat * fixed[i];
		}
		m_bounds.insert(std::make_pair(spr, fixed));
	} else if (itr != m_bounds.end() && disable) {
		m_bounds.erase(itr);
	}

	// create nodes
	std::vector<sm::vec2> all_bound;
	all_bound.push_back(sm::vec2(m_region.xmin, m_region.ymin));
	all_bound.push_back(sm::vec2(m_region.xmin, m_region.ymax));
	all_bound.push_back(sm::vec2(m_region.xmax, m_region.ymax));
	all_bound.push_back(sm::vec2(m_region.xmax, m_region.ymin));

	std::vector<sm::vec2> lines;
	std::vector<std::vector<sm::vec2> > loops;
	for (itr = m_bounds.begin(); itr != m_bounds.end(); ++itr) {
		loops.push_back(itr->second);
	}
	sm::triangulate_lines_and_loops(all_bound, lines, loops, m_tris);
}

void PathNavMesh::QueryRoute(const sm::vec2& start, const sm::vec2& end)
{
	
}

void PathNavMesh::DebugDraw() const
{
	for (int i = 0, n = m_tris.size(); i < n; ) {
		std::vector<sm::vec2> polyline;
		polyline.push_back(m_tris[i++]);
		polyline.push_back(m_tris[i++]);
		polyline.push_back(m_tris[i++]);
		s2::RVG::SetColor(ee::LIGHT_RED);
		s2::RVG::Polyline(polyline, true);
	}
}

sm::vec2 PathNavMesh::TransIDToPos(int id) const
{
	sm::vec2 ret;

	// todo 

	return ret;
}

}
}