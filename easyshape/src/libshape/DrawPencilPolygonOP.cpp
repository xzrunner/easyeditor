#include "DrawPencilPolygonOP.h"
#include "DrawPencilPolygonCMPT.h"
#include "PolygonShape.h"

#include <ee/DouglasPeucker.h>
#include <ee/panel_msg.h>
#include <ee/shape_msg.h>
#include <ee/MultiShapesImpl.h>
#include <ee/PolygonClipper.h>
#include <ee/FetchAllVisitor.h>

namespace eshape
{

DrawPencilPolygonOP::DrawPencilPolygonOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
										 ee::MultiShapesImpl* shapes_impl,
										 ee::OneFloatValue* simplify,
										 DrawPencilPolygonCMPT* cmpt)
	: DrawCurveOP(wnd, stage)
	, m_shapes_impl(shapes_impl)
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
		std::vector<sm::vec2> simplified;
		ee::DouglasPeucker::Do(m_curve, m_simplify->GetValue(), simplified);
		NewPolygon(simplified);
		Clear();

		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

void DrawPencilPolygonOP::NewPolygon(const std::vector<sm::vec2>& poly)
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

void DrawPencilPolygonOP::UnionPolygon(const std::vector<sm::vec2>& poly)
{
	std::vector<std::vector<sm::vec2> > sub_points;
	PrepareSubjectPaths(sub_points);

	std::vector<std::vector<sm::vec2> > result = 
		ee::PolygonClipper::Union(sub_points, poly);

	ReplacePolygons(result);
}

void DrawPencilPolygonOP::DifferencePolygon(const std::vector<sm::vec2>& poly)
{
	std::vector<std::vector<sm::vec2> > sub_points;
	PrepareSubjectPaths(sub_points);

	std::vector<std::vector<sm::vec2> > result = 
		ee::PolygonClipper::Difference(sub_points, poly);

	ReplacePolygons(result);
}

void DrawPencilPolygonOP::IntersectionPolygon(const std::vector<sm::vec2>& poly)
{
	std::vector<std::vector<sm::vec2> > sub_points;
	PrepareSubjectPaths(sub_points);

	std::vector<std::vector<sm::vec2> > result = 
		ee::PolygonClipper::Intersection(sub_points, poly);

	ReplacePolygons(result);
}

void DrawPencilPolygonOP::XorPolygon(const std::vector<sm::vec2>& poly)
{
	std::vector<std::vector<sm::vec2> > sub_points;
	PrepareSubjectPaths(sub_points);

	std::vector<std::vector<sm::vec2> > result = 
		ee::PolygonClipper::Xor(sub_points, poly);

	ReplacePolygons(result);
}

void DrawPencilPolygonOP::PrepareSubjectPaths(std::vector<std::vector<sm::vec2> >& paths) const
{
	std::vector<PolygonShape*> shapes;
	m_shapes_impl->TraverseShapes(ee::FetchAllVisitor<PolygonShape>(shapes));

	paths.clear();
	paths.resize(shapes.size());
	for (int i = 0, n = shapes.size(); i < n; ++i) {
		paths[i] = shapes[i]->GetVertices();
	}
}

void DrawPencilPolygonOP::ReplacePolygons(const std::vector<std::vector<sm::vec2> >& paths)
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