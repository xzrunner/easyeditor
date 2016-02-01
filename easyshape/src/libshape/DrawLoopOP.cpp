#include "DrawLoopOP.h"
#include "ChainShape.h"

namespace eshape
{

DrawLoopOP::DrawLoopOP(wxWindow* wnd, ee::EditPanelImpl* stage,
					   ee::MultiShapesImpl* shapesImpl)
	: DrawPolylineOP(wnd, stage, false)
{
}

bool DrawLoopOP::OnMouseLeftDClick(int x, int y)
{
	if (DrawPolylineOP::OnMouseLeftDClick(x, y)) return true;

	ee::Shape* shape = new ChainShape(m_polyline, true);
	ee::InsertShapeSJ::Instance()->Insert(shape);
	shape->Release();

	m_polyline.clear();
	m_currPos.SetInvalid();

	return false;
}

}