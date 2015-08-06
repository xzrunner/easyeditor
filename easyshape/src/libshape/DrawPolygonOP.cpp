#include "DrawPolygonOP.h"
#include "PolygonShape.h"

namespace libshape
{

DrawPolygonOP::DrawPolygonOP(wxWindow* wnd, d2d::EditPanelImpl* stage, 
							 d2d::MultiShapesImpl* shapesImpl)
	: DrawPolylineOP(wnd, stage, false)
	, m_shapesImpl(shapesImpl)
{
	m_cursor = wxCursor(wxCURSOR_PENCIL);
}

bool DrawPolygonOP::OnMouseLeftDClick(int x, int y)
{
	if (DrawPolylineOP::OnMouseLeftDClick(x, y)) return true;

	if (m_polyline.size() >= 3)
	{
		m_shapesImpl->InsertShape(new PolygonShape(m_polyline));
		m_polyline.clear();
		m_currPos.setInvalid();
	}

	return false;
}

}