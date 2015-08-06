#include "DrawLoopOP.h"
#include "ChainShape.h"

namespace libshape
{

DrawLoopOP::DrawLoopOP(wxWindow* wnd, d2d::EditPanelImpl* stage, 
					   d2d::MultiShapesImpl* shapesImpl)
	: DrawPolylineOP(wnd, stage, false)
	, m_shapesImpl(shapesImpl)
{
}

bool DrawLoopOP::OnMouseLeftDClick(int x, int y)
{
	if (DrawPolylineOP::OnMouseLeftDClick(x, y)) return true;

	d2d::IShape* shape = new ChainShape(m_polyline, true);
	m_shapesImpl->InsertShape(shape);
	shape->Release();

	m_polyline.clear();
	m_currPos.setInvalid();

	return false;
}

}