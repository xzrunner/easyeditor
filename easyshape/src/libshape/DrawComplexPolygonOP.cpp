#include "DrawComplexPolygonOP.h"
#include "ComplexPolygonShape.h"

namespace libshape
{

DrawComplexPolygonOP::DrawComplexPolygonOP(wxWindow* wnd, d2d::EditPanelImpl* stage, 
										   d2d::MultiShapesImpl* shapesImpl)
	: DrawPolylineOP(wnd, stage, false)
	, m_shapesImpl(shapesImpl)
{
}

bool DrawComplexPolygonOP::OnMouseLeftDClick(int x, int y)
{
	if (DrawPolylineOP::OnMouseLeftDClick(x, y)) return true;

	if (m_polyline.size() < 3) {
		return false;
	}

	ComplexPolygonShape* cpoly = CreateComplexPoly(m_polyline);
	if (cpoly) {
		d2d::InsertShapeSJ::Instance()->Insert(cpoly);
		cpoly->Release();
	}

	m_polyline.clear();
	m_currPos.setInvalid();

	return false;
}

ComplexPolygonShape* DrawComplexPolygonOP::CreateComplexPoly(const std::vector<d2d::Vector>& polyline)
{
	std::vector<PolygonShape*> polygon_shapes;
	m_shapesImpl->TraverseShapes(d2d::FetchAllVisitor<PolygonShape>(polygon_shapes));

	for (int i = 0, n = polygon_shapes.size(); i < n; ++i)
	{
		PolygonShape* poly = polygon_shapes[i];
		const std::vector<d2d::Vector>& outline = poly->GetVertices();
		if (!d2d::Math::IsPolygonInPolygon(polyline, outline)) {
			continue;
		}

		ComplexPolygonShape* new_cpoly = NULL;
		if (ComplexPolygonShape* cpoly = dynamic_cast<ComplexPolygonShape*>(poly))
		{
			const std::vector<std::vector<d2d::Vector> >& holes = cpoly->GetHoles();
			for (int i = 0, n = holes.size(); i < n; ++i) {
				if (d2d::Math::isPolylineIntersectPolyline(holes[i], polyline)) {
					return NULL;
				}
			}

			std::vector<std::vector<d2d::Vector> > new_holes = holes;
			new_holes.push_back(polyline);

			new_cpoly = new ComplexPolygonShape(poly->GetVertices(), new_holes);
			d2d::RemoveShapeSJ::Instance()->Remove(cpoly);
		}
		else
		{
			std::vector<std::vector<d2d::Vector> > holes;
			holes.push_back(polyline);

			new_cpoly = new ComplexPolygonShape(poly->GetVertices(), holes);
			d2d::RemoveShapeSJ::Instance()->Remove(poly);
		}

		return new_cpoly;
	}

	std::vector<std::vector<d2d::Vector> > holes;
	return new ComplexPolygonShape(polyline, holes);
}

}