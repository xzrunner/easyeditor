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
	} else if (type == e_union) {
		UnionPolygon(poly);
	} else if (type == e_difference) {
		DifferencePolygon(poly);
	} else if (type == e_intersection) {
		IntersectionPolygon(poly);
	} else if (type == e_xor) {
		XorPolygon(poly);
	}
}

void DrawPencilPolygonOP::UnionPolygon(const std::vector<d2d::Vector>& poly)
{
	std::vector<std::vector<d2d::Vector> > sub_points;
	PrepareSubjectPaths(sub_points);

	std::vector<std::vector<d2d::Vector> > result = 
		d2d::PolygonClipper::Union(sub_points, poly);

	ReplacePolygons(result);
}

void DrawPencilPolygonOP::DifferencePolygon(const std::vector<d2d::Vector>& poly)
{
	std::vector<std::vector<d2d::Vector> > sub_points;
	PrepareSubjectPaths(sub_points);

	std::vector<std::vector<d2d::Vector> > result = 
		d2d::PolygonClipper::Difference(sub_points, poly);

	ReplacePolygons(result);
}

void DrawPencilPolygonOP::IntersectionPolygon(const std::vector<d2d::Vector>& poly)
{
	std::vector<std::vector<d2d::Vector> > sub_points;
	PrepareSubjectPaths(sub_points);

	std::vector<std::vector<d2d::Vector> > result = 
		d2d::PolygonClipper::Intersection(sub_points, poly);

	ReplacePolygons(result);
}

void DrawPencilPolygonOP::XorPolygon(const std::vector<d2d::Vector>& poly)
{
	std::vector<std::vector<d2d::Vector> > sub_points;
	PrepareSubjectPaths(sub_points);

	std::vector<std::vector<d2d::Vector> > result = 
		d2d::PolygonClipper::Xor(sub_points, poly);

	ReplacePolygons(result);
}

void DrawPencilPolygonOP::PrepareSubjectPaths(std::vector<std::vector<d2d::Vector> >& paths) const
{
	std::vector<PolygonShape*> shapes;
	m_shapesImpl->traverseShapes(d2d::FetchAllVisitor<PolygonShape>(shapes));

	paths.clear();
	paths.resize(shapes.size());
	for (int i = 0, n = shapes.size(); i < n; ++i) {
		paths[i] = shapes[i]->GetVertices();
	}
}

void DrawPencilPolygonOP::ReplacePolygons(const std::vector<std::vector<d2d::Vector> >& paths)
{
	m_shapesImpl->clearShapes();
	for (int i = 0, n = paths.size(); i < n; ++i) {
		m_shapesImpl->insertShape(new PolygonShape(paths[i]));
	}
}


}