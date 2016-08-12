#include "DrawLoopOP.h"
#include "ChainShape.h"

#include <ee/shape_msg.h>

namespace eshape
{

DrawLoopOP::DrawLoopOP(wxWindow* wnd, ee::EditPanelImpl* stage,
					   ee::MultiShapesImpl* shapes_impl)
	: DrawPolylineOP(wnd, stage, false)
{
}

bool DrawLoopOP::OnMouseLeftDClick(int x, int y)
{
	if (DrawPolylineOP::OnMouseLeftDClick(x, y)) return true;

	ee::Shape* shape = new ChainShape(m_polyline, true);
	ee::InsertShapeSJ::Instance()->Insert(shape);
	shape->RemoveReference();

	m_polyline.clear();
	m_curr_pos.MakeInvalid();

	return false;
}

}