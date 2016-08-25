//#include "ComplexPolygonShape.h"
//#include "Material.h"
//
//#include <ee/Math2D.h>
//#include <ee/SettingData.h>
//
//#include <sprite2/S2_RVG.h>
//
//namespace eshape
//{
//
//ComplexPolygonShape::ComplexPolygonShape()
//{
//}
//
//ComplexPolygonShape::ComplexPolygonShape(const ComplexPolygonShape& poly)
//	: PolygonShape(poly)
//{
//}
//
//ComplexPolygonShape::ComplexPolygonShape(const std::vector<sm::vec2>& outline, 
//										 const std::vector<std::vector<sm::vec2> >& holes)
//	: PolygonShape(outline)
//	, m_holes(holes)
//{
//	m_material->BuildSetHoles(holes);
//	m_material->BuildEnd();
//}
//
//bool ComplexPolygonShape::IsContain(const sm::vec2& pos) const
//{
//	if (PolygonShape::IsContain(pos)) {
//		for (int i = 0, n = m_holes.size(); i < n; ++i) {
//			if (sm::is_point_in_area(pos, m_holes[i])) {
//				return false;
//			}
//		}
//		return true;
//	}
//	return false;
//}
//
//bool ComplexPolygonShape::IsIntersect(const sm::rect& rect) const
//{
//	return ee::Math2D::IsPolygonIntersectRect(m_vertices, rect);
//}
//
//void ComplexPolygonShape::Translate(const sm::vec2& offset)
//{
//	PolygonShape::Translate(offset);
//
//	for (int i = 0, n = m_holes.size(); i < n; ++i) {
//		for (int j = 0, m = m_holes[i].size(); j < m; ++j) {
//			m_holes[i][j] += offset;
//		}
//	}
//}
//
//void ComplexPolygonShape::Draw(const sm::mat4& mt, const s2::RenderColor& color) const
//{
//	PolygonShape::Draw(mt, color);
//
//	if (ee::SettingData::draw_poly_bound) {
//		float len = sm::mat_trans_len(ee::SettingData::ctl_pos_sz, mt);
//		for (int i = 0, n = m_holes.size(); i < n; ++i) {
//			std::vector<sm::vec2> vertices;
//			sm::trans_vertices(mt, m_holes[i], vertices);
//			s2::RVG::SetColor(color.mul);
//			s2::RVG::Polyline(vertices, m_loop);
//			if (len != 0) {
//				s2::RVG::SetColor(s2::Color(102, 204, 102));
//				s2::RVG::Circles(vertices, len, true);
//			}
//		}
//	} 
//}
//
//}