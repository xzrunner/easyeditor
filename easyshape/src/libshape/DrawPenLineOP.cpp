#include "DrawPenLineOP.h"
#include "ChainShape.h"

#include <ee/shape_msg.h>

namespace eshape
{

DrawPenLineOP::DrawPenLineOP(wxWindow* wnd, ee::EditPanelImpl* stage,
							 ee::MultiShapesImpl* shapes_impl)
	: DrawPolylineOP(wnd, stage, false)
{
}

bool DrawPenLineOP::OnMouseLeftDClick(int x, int y)
{
	if (DrawPolylineOP::OnMouseLeftDClick(x, y)) return true;

	ee::InsertShapeSJ::Instance()->Insert(new ChainShape(m_polyline, false));
	m_polyline.clear();
	m_curr_pos.SetInvalid();

	return false;
}

}