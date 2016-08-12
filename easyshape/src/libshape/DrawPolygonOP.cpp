#include "DrawPolygonOP.h"
#include "PolygonShape.h"

#include <ee/shape_msg.h>

namespace eshape
{

DrawPolygonOP::DrawPolygonOP(wxWindow* wnd, ee::EditPanelImpl* stage,
							 ee::MultiShapesImpl* shapes_impl)
	: DrawPolylineOP(wnd, stage, false)
{
	m_cursor = wxCursor(wxCURSOR_PENCIL);
}

bool DrawPolygonOP::OnMouseLeftDClick(int x, int y)
{
	if (DrawPolylineOP::OnMouseLeftDClick(x, y)) return true;

	if (m_polyline.size() >= 3)
	{
		PolygonShape* poly = new PolygonShape(m_polyline);
		ee::InsertShapeSJ::Instance()->Insert(poly);
		poly->RemoveReference();
		m_polyline.clear();
		m_curr_pos.MakeInvalid();
	}

	return false;
}

}