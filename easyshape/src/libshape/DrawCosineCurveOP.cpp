#include "DrawCosineCurveOP.h"
#include "CosineShape.h"

namespace libshape
{

DrawCosineCurveOP::DrawCosineCurveOP(wxWindow* wnd, ee::EditPanelImpl* stage)
	: DrawPolylineOP(wnd, stage, false)
{
}

bool DrawCosineCurveOP::OnMouseLeftDClick(int x, int y)
{
	if (DrawPolylineOP::OnMouseLeftDClick(x, y)) return true;

	CosineShape* curve = new CosineShape(m_polyline);
	curve->setMidPoints();

	ee::InsertShapeSJ::Instance()->Insert(curve);

	m_polyline.clear();
	m_currPos.SetInvalid();

	return false;
}

}