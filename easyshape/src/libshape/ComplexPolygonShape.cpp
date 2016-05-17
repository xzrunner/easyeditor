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

ComplexPolygonShape::ComplexPolygonShape(const std::vector<sm::vec2>& outline, 
										 const std::vector<std::vector<sm::vec2> >& holes)
	: PolygonShape(outline)
	, m_holes(holes)
{
	m_material->BuildSetHoles(holes);
	m_material->BuildEnd();
}

bool ComplexPolygonShape::IsContain(const sm::vec2& pos) const
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

bool ComplexPolygonShape::IsIntersect(const sm::rect& rect) const
{
	return ee::Math2D::IsPolygonIntersectRect(m_vertices, rect);
}

void ComplexPolygonShape::Translate(const sm::vec2& offset)
{
	PolygonShape::Translate(offset);

	for (int i = 0, n = m_holes.size(); i < n; ++i) {
		for (int j = 0, m = m_holes[i].size(); j < m; ++j) {
			m_holes[i][j] += offset;
		}
	}
}

void ComplexPolygonShape::Draw(const sm::mat4& mt, const s2::RenderColor& color) const
{
	PolygonShape::Draw(mt, color);

	if (ee::SettingData::draw_poly_bound) {
		float len = ee::Math2D::TransLen(ee::SettingData::ctl_pos_sz, mt);
		for (int i = 0, n = m_holes.size(); i < n; ++i) {
			std::vector<sm::vec2> vertices;
			ee::Math2D::TransVertices(mt, m_holes[i], vertices);
			ee::RVG::Color(color.mul);
			ee::RVG::Polyline(vertices, m_loop);
			if (len != 0) {
				ee::RVG::Color(s2::Color(102, 204, 102));
				ee::RVG::Circles(vertices, len, true);
			}
		}
	} 
}

}