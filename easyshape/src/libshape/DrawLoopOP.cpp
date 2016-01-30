#include "DrawLoopOP.h"
#include "ChainShape.h"

namespace libshape
{

DrawLoopOP::DrawLoopOP(wxWindow* wnd, d2d::EditPanelImpl* stage,
					   d2d::MultiShapesImpl* shapesImpl)
	: DrawPolylineOP(wnd, stage, false)
{
}

bool DrawLoopOP::OnMouseLeftDClick(int x, int y)
{
	if (DrawPolylineOP::OnMouseLeftDClick(x, y)) return true;

	d2d::Shape* shape = new ChainShape(m_polyline, true);
	d2d::InsertShapeSJ::Instance()->Insert(shape);
	shape->Release();

	m_polyline.clear();
	m_currPos.SetInvalid();

	return false;
}

}