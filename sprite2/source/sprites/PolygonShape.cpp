//#include "PolygonShape.h"
//
//namespace s2
//{
//
//PolygonShape::PolygonShape()
//	: m_poly(NULL)
//{
//}
//
//PolygonShape::PolygonShape(const PolygonShape& poly)
//	: eshape::PolylineShape(poly)
//	, s2::PolylineShape(poly)
//{
//	if (poly.m_poly) {
//		poly.m_poly->AddReference();
//		m_poly = poly.m_poly;
//	}
//}
//
//PolygonShape::PolygonShape(const std::vector<sm::vec2>& vertices)
//	: s2::PolylineShape(vertices, true)
//	, m_poly(NULL)
//{
//	SetMaterialColor(ee::LIGHT_GREEN);
//}
//
//PolygonShape& PolygonShape::operator = (const PolygonShape& poly)
//{
//	m_vertices = poly.m_vertices;
//	m_closed = true;
//
//	if (poly.m_poly) {
//		m_poly = poly.m_poly;
//		poly.m_poly->AddReference();
//	} else {
//		m_poly = NULL;
//	}
//	return *this;
//}
//
//PolygonShape::~PolygonShape()
//{
//	if (m_poly) {
//		m_poly->RemoveReference();
//	}
//	ClearUserData(true);
//}
//
//}