//#include "DrawComplexPolygonOP.h"
//#include "ComplexPolygonShape.h"
//
//#include <ee/shape_msg.h>
//#include <ee/MultiShapesImpl.h>
//#include <ee/FetchAllVisitor.h>
//#include <ee/Math2D.h>
//
//namespace eshape
//{
//
//DrawComplexPolygonOP::DrawComplexPolygonOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
//										   ee::MultiShapesImpl* shapes_impl)
//	: DrawPolylineOP(wnd, stage, false)
//	, m_shapes_impl(shapes_impl)
//{
//}
//
//bool DrawComplexPolygonOP::OnMouseLeftDClick(int x, int y)
//{
//	if (DrawPolylineOP::OnMouseLeftDClick(x, y)) return true;
//
//	if (m_polyline.size() < 3) {
//		return false;
//	}
//
//	ComplexPolygonShape* cpoly = CreateComplexPoly(m_polyline);
//	if (cpoly) {
//		ee::InsertShapeSJ::Instance()->Insert(cpoly);
//		cpoly->RemoveReference();
//	}
//
//	m_polyline.clear();
//	m_curr_pos_valid = false;
//
//	return false;
//}
//
//ComplexPolygonShape* DrawComplexPolygonOP::CreateComplexPoly(const std::vector<sm::vec2>& polyline)
//{
//	std::vector<PolygonShape*> polygon_shapes;
//	m_shapes_impl->TraverseShapes(ee::FetchAllVisitor<PolygonShape>(polygon_shapes));
//
//	for (int i = 0, n = polygon_shapes.size(); i < n; ++i)
//	{
//		PolygonShape* poly = polygon_shapes[i];
//		const std::vector<sm::vec2>& outline = poly->GetVertices();
//		if (!ee::Math2D::IsPolygonInPolygon(polyline, outline)) {
//			continue;
//		}
//
//		ComplexPolygonShape* new_cpoly = NULL;
//		if (ComplexPolygonShape* cpoly = dynamic_cast<ComplexPolygonShape*>(poly))
//		{
//			const std::vector<std::vector<sm::vec2> >& holes = cpoly->GetHoles();
//			for (int i = 0, n = holes.size(); i < n; ++i) {
//				if (ee::Math2D::isPolylineIntersectPolylinI(holes[i], polyline)) {
//					return NULL;
//				}
//			}
//
//			std::vector<std::vector<sm::vec2> > new_holes = holes;
//			new_holes.push_back(polyline);
//
//			new_cpoly = new ComplexPolygonShape(poly->GetVertices(), new_holes);
//			ee::RemoveShapeSJ::Instance()->Remove(cpoly);
//		}
//		else
//		{
//			std::vector<std::vector<sm::vec2> > holes;
//			holes.push_back(polyline);
//
//			new_cpoly = new ComplexPolygonShape(poly->GetVertices(), holes);
//			ee::RemoveShapeSJ::Instance()->Remove(poly);
//		}
//
//		return new_cpoly;
//	}
//
//	std::vector<std::vector<sm::vec2> > holes;
//	return new ComplexPolygonShape(polyline, holes);
//}
//
//}