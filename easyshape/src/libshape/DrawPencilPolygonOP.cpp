#include "DrawPencilPolygonOP.h"
#include "DrawPencilPolygonCMPT.h"
#include "PolygonShape.h"

namespace eshape
{

DrawPencilPolygonOP::DrawPencilPolygonOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
										 ee::MultiShapesImpl* shapesImpl,
										 ee::OneFloatValue* simplify,
										 DrawPencilPolygonCMPT* cmpt)
	: DrawCurveOP(wnd, stage)
	, m_shapesImpl(shapesImpl)
	, m_simplify(simplify)
	, m_cmpt(cmpt)
{
}

bool DrawPencilPolygonOP::OnKeyDown(int keyCode)
{
	if (DrawCurveOP::OnKeyDown(keyCode)) {
		return true;
	}

	switch (keyCode)
	{
	case WXK_SHIFT:
		m_cmpt->SetSelection("union");
		break;
	case WXK_ALT:
		m_cmpt->SetSelection("difference");
		break;
	case WXK_ESCAPE:
		m_cmpt->SetSelection("normal");
		break;
	}

	return false;
}

bool DrawPencilPolygonOP::OnMouseLeftUp(int x, int y)
{
	if (DrawCurveOP::OnMouseLeftUp(x, y)) return true;

	if (!m_curve.empty())
	{
		std::vector<ee::Vector> simplified;
		ee::DouglasPeucker::Do(m_curve, m_simplify->GetValue(), simplified);
		NewPolygon(simplified);
		Clear();

		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

void DrawPencilPolygonOP::NewPolygon(const std::vector<ee::Vector>& poly)
{
	Type type = (Type)m_cmpt->GetSelected();
	if (type == e_normal) {
		ee::InsertShapeSJ::Instance()->Insert(new PolygonShape(poly));
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

void DrawPencilPolygonOP::UnionPolygon(const std::vector<ee::Vector>& poly)
{
	std::vector<std::vector<ee::Vector> > sub_points;
	PrepareSubjectPaths(sub_points);

	std::vector<std::vector<ee::Vector> > result = 
		ee::PolygonClipper::Union(sub_points, poly);

	ReplacePolygons(result);
}

void DrawPencilPolygonOP::DifferencePolygon(const std::vector<ee::Vector>& poly)
{
	std::vector<std::vector<ee::Vector> > sub_points;
	PrepareSubjectPaths(sub_points);

	std::vector<std::vector<ee::Vector> > result = 
		ee::PolygonClipper::Difference(sub_points, poly);

	ReplacePolygons(result);
}

void DrawPencilPolygonOP::IntersectionPolygon(const std::vector<ee::Vector>& poly)
{
	std::vector<std::vector<ee::Vector> > sub_points;
	PrepareSubjectPaths(sub_points);

	std::vector<std::vector<ee::Vector> > result = 
		ee::PolygonClipper::Intersection(sub_points, poly);

	ReplacePolygons(result);
}

void DrawPencilPolygonOP::XorPolygon(const std::vector<ee::Vector>& poly)
{
	std::vector<std::vector<ee::Vector> > sub_points;
	PrepareSubjectPaths(sub_points);

	std::vector<std::vector<ee::Vector> > result = 
		ee::PolygonClipper::Xor(sub_points, poly);

	ReplacePolygons(result);
}

void DrawPencilPolygonOP::PrepareSubjectPaths(std::vector<std::vector<ee::Vector> >& paths) const
{
	std::vector<PolygonShape*> shapes;
	m_shapesImpl->TraverseShapes(ee::FetchAllVisitor<PolygonShape>(shapes));

	paths.clear();
	paths.resize(shapes.size());
	for (int i = 0, n = shapes.size(); i < n; ++i) {
		paths[i] = shapes[i]->GetVertices();
	}
}

void DrawPencilPolygonOP::ReplacePolygons(const std::vector<std::vector<ee::Vector> >& paths)
{
	// for shadow, fixme!
	if (paths.size() > 1) {
		return;
	}

	ee::ClearShapeSJ::Instance()->Clear();
	for (int i = 0, n = paths.size(); i < n; ++i) {
		ee::InsertShapeSJ::Instance()->Insert(new PolygonShape(paths[i]));
	}
}


}