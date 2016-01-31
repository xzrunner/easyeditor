#include "DrawPenLineOP.h"
#include "ChainShape.h"

namespace libshape
{

DrawPenLineOP::DrawPenLineOP(wxWindow* wnd, ee::EditPanelImpl* stage,
							 ee::MultiShapesImpl* shapesImpl)
	: DrawPolylineOP(wnd, stage, false)
{
}

bool DrawPenLineOP::OnMouseLeftDClick(int x, int y)
{
	if (DrawPolylineOP::OnMouseLeftDClick(x, y)) return true;

	ee::InsertShapeSJ::Instance()->Insert(new ChainShape(m_polyline, false));
	m_polyline.clear();
	m_currPos.SetInvalid();

	return false;
}

}