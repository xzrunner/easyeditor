#include "DrawCosineCurveOP.h"
#include "CosineShape.h"

namespace libshape
{

DrawCosineCurveOP::DrawCosineCurveOP(wxWindow* wnd, d2d::EditPanelImpl* stage, 
									 d2d::MultiShapesImpl* shapesImpl)
	: DrawPolylineOP(wnd, stage, false)
	, m_shapesImpl(shapesImpl)
{
}

bool DrawCosineCurveOP::OnMouseLeftDClick(int x, int y)
{
	if (DrawPolylineOP::OnMouseLeftDClick(x, y)) return true;

	CosineShape* curve = new CosineShape(m_polyline);
	curve->setMidPoints();

	m_shapesImpl->InsertShape(curve);
	m_polyline.clear();
	m_currPos.setInvalid();

	return false;
}

}