#include "DrawPolygonOP.h"
#include "PolygonShape.h"

namespace eshape
{

DrawPolygonOP::DrawPolygonOP(wxWindow* wnd, ee::EditPanelImpl* stage,
							 ee::MultiShapesImpl* shapesImpl)
	: DrawPolylineOP(wnd, stage, false)
{
	m_cursor = wxCursor(wxCURSOR_PENCIL);
}

bool DrawPolygonOP::OnMouseLeftDClick(int x, int y)
{
	if (DrawPolylineOP::OnMouseLeftDClick(x, y)) return true;

	if (m_polyline.size() >= 3)
	{
		ee::InsertShapeSJ::Instance()->Insert(new PolygonShape(m_polyline));
		m_polyline.clear();
		m_currPos.SetInvalid();
	}

	return false;
}

}