#include "DrawPenLineOP.h"
#include "ChainShape.h"

namespace libshape
{

DrawPenLineOP::DrawPenLineOP(wxWindow* wnd, d2d::EditPanelImpl* stage, 
							 d2d::MultiShapesImpl* shapesImpl)
	: DrawPolylineOP(wnd, stage, false)
	, m_shapesImpl(shapesImpl)
{
}

bool DrawPenLineOP::OnMouseLeftDClick(int x, int y)
{
	if (DrawPolylineOP::OnMouseLeftDClick(x, y)) return true;

	m_shapesImpl->InsertShape(new ChainShape(m_polyline, false));
	m_polyline.clear();
	m_currPos.setInvalid();

	return false;
}

}