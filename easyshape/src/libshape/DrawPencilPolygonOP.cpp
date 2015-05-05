#include "DrawPencilPolygonOP.h"
#include "DrawPencilPolygonCMPT.h"
#include "PolygonShape.h"

namespace libshape
{

DrawPencilPolygonOP::DrawPencilPolygonOP(d2d::EditPanel* editPanel, 
										 d2d::MultiShapesImpl* shapesImpl,
										 d2d::OneFloatValue* simplify,
										 DrawPencilPolygonCMPT* cmpt)
	: DrawCurveOP(editPanel)
	, m_shapesImpl(shapesImpl)
	, m_simplify(simplify)
	, m_cmpt(cmpt)
{
}

bool DrawPencilPolygonOP::onMouseLeftUp(int x, int y)
{
	if (DrawCurveOP::onMouseLeftUp(x, y)) return true;

	if (!m_curve.empty())
	{
		std::vector<d2d::Vector> simplified;
		d2d::DouglasPeucker::implement(m_curve, m_simplify->GetValue(), simplified);
		NewPolygon(simplified);
		clear();

		m_editPanel->Refresh();
	}

	return false;
}

void DrawPencilPolygonOP::NewPolygon(const std::vector<d2d::Vector>& poly)
{
	Type type = (Type)m_cmpt->GetSelected();
	if (type == e_normal) {
		m_shapesImpl->insertShape(new PolygonShape(poly));
	} else if (type == e_add) {
		UnionPolygon(poly);
	} else if (type == e_del) {
		DifferencePolygon(poly);
	}
}

void DrawPencilPolygonOP::UnionPolygon(const std::vector<d2d::Vector>& poly)
{
	std::vector<PolygonShape*> shapes;
	m_shapesImpl->traverseShapes(d2d::FetchAllVisitor<PolygonShape>(shapes));
	
	std::vector<std::vector<d2d::Vector> > sub_points;
	sub_points.resize(shapes.size());
	for (int i = 0, n = shapes.size(); i < n; ++i) {
		sub_points[i] = shapes[i]->GetVertices();
	}

	std::vector<std::vector<d2d::Vector> > result = 
		d2d::PolygonClipper::Union(sub_points, poly);
	m_shapesImpl->clearShapes();
	for (int i = 0, n = result.size(); i < n; ++i) {
		m_shapesImpl->insertShape(new PolygonShape(result[i]));
	}
}

void DrawPencilPolygonOP::DifferencePolygon(const std::vector<d2d::Vector>& poly)
{
	std::vector<PolygonShape*> shapes;
	m_shapesImpl->traverseShapes(d2d::FetchAllVisitor<PolygonShape>(shapes));

	std::vector<std::vector<d2d::Vector> > sub_points;
	sub_points.resize(shapes.size());
	for (int i = 0, n = shapes.size(); i < n; ++i) {
		sub_points[i] = shapes[i]->GetVertices();
	}

	std::vector<std::vector<d2d::Vector> > result = 
		d2d::PolygonClipper::Difference(sub_points, poly);
	m_shapesImpl->clearShapes();
	for (int i = 0, n = result.size(); i < n; ++i) {
		m_shapesImpl->insertShape(new PolygonShape(result[i]));
	}
}

}