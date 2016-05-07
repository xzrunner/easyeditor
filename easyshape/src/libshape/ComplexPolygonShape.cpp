#include "ComplexPolygonShape.h"
#include "Material.h"

#include <ee/Math2D.h>
#include <ee/EE_RVG.h>
#include <ee/SettingData.h>

namespace eshape
{

ComplexPolygonShape::ComplexPolygonShape()
{
}

ComplexPolygonShape::ComplexPolygonShape(const ComplexPolygonShape& poly)
	: PolygonShape(poly)
{
}

ComplexPolygonShape::ComplexPolygonShape(const std::vector<ee::Vector>& outline, 
										 const std::vector<std::vector<ee::Vector> >& holes)
	: PolygonShape(outline)
	, m_holes(holes)
{
	m_material->BuildSetHoles(holes);
	m_material->BuildEnd();
}

bool ComplexPolygonShape::IsContain(const ee::Vector& pos) const
{
	if (PolygonShape::IsContain(pos)) {
		for (int i = 0, n = m_holes.size(); i < n; ++i) {
			if (ee::Math2D::IsPointInArea(pos, m_holes[i])) {
				return false;
			}
		}
		return true;
	}
	return false;
}

bool ComplexPolygonShape::IsIntersect(const ee::Rect& rect) const
{
	return ee::Math2D::IsPolygonIntersectRect(m_vertices, rect);
}

void ComplexPolygonShape::Translate(const ee::Vector& offset)
{
	PolygonShape::Translate(offset);

	for (int i = 0, n = m_holes.size(); i < n; ++i) {
		for (int j = 0, m = m_holes[i].size(); j < m; ++j) {
			m_holes[i][j] += offset;
		}
	}
}

void ComplexPolygonShape::Draw(const sm::mat4& mt, const ee::RenderColor& color) const
{
	PolygonShape::Draw(mt, color);

	if (ee::SettingData::draw_poly_bound) {
		float len = ee::Math2D::TransLen(ee::SettingData::ctl_pos_sz, mt);
		for (int i = 0, n = m_holes.size(); i < n; ++i) {
			std::vector<ee::Vector> vertices;
			ee::Math2D::TransVertices(mt, m_holes[i], vertices);
			ee::RVG::Color(color.multi);
			ee::RVG::Polyline(vertices, m_loop);
			if (len != 0) {
				ee::RVG::Color(ee::Colorf(0.4f, 0.8f, 0.4f));
				ee::RVG::Circles(vertices, len, true);
			}
		}
	} 
}

}