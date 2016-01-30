#include "ComplexPolygonShape.h"
#include "Material.h"

namespace libshape
{

ComplexPolygonShape::ComplexPolygonShape()
{
}

ComplexPolygonShape::ComplexPolygonShape(const ComplexPolygonShape& poly)
	: PolygonShape(poly)
{
}

ComplexPolygonShape::ComplexPolygonShape(const std::vector<d2d::Vector>& outline, 
										 const std::vector<std::vector<d2d::Vector> >& holes)
	: PolygonShape(outline)
	, m_holes(holes)
{
	m_material->BuildSetHoles(holes);
	m_material->BuildEnd();
}

bool ComplexPolygonShape::IsContain(const d2d::Vector& pos) const
{
	if (PolygonShape::IsContain(pos)) {
		for (int i = 0, n = m_holes.size(); i < n; ++i) {
			if (d2d::Math2D::IsPointInArea(pos, m_holes[i])) {
				return false;
			}
		}
		return true;
	}
	return false;
}

bool ComplexPolygonShape::IsIntersect(const d2d::Rect& rect) const
{
	return d2d::Math2D::IsPolygonIntersectRect(m_vertices, rect);
}

void ComplexPolygonShape::Translate(const d2d::Vector& offset)
{
	PolygonShape::Translate(offset);

	for (int i = 0, n = m_holes.size(); i < n; ++i) {
		for (int j = 0, m = m_holes[i].size(); j < m; ++j) {
			m_holes[i][j] += offset;
		}
	}
}

void ComplexPolygonShape::Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color) const
{
	PolygonShape::Draw(mt, color);

	if (d2d::SettingData::draw_poly_bound) {
		for (int i = 0, n = m_holes.size(); i < n; ++i) {
			d2d::PrimitiveDraw::DrawPolyline(mt, m_holes[i], color.multi, m_isLoop);
			if (d2d::SettingData::ctl_pos_sz != 0) {
				d2d::PrimitiveDraw::DrawCircles(m_holes[i], d2d::SettingData::ctl_pos_sz, true, 2, d2d::Colorf(0.4f, 0.8f, 0.4f));
			}
		}
	} 
}

}