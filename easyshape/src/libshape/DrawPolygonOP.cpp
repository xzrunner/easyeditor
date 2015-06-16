#include "DrawPolygonOP.h"
#include "PolygonShape.h"

namespace libshape
{

DrawPolygonOP::DrawPolygonOP(d2d::EditPanel* editPanel, 
									 d2d::MultiShapesImpl* shapesImpl)
	: DrawPolylineOP(editPanel, false)
	, m_shapesImpl(shapesImpl)
{
}

bool DrawPolygonOP::OnMouseLeftDClick(int x, int y)
{
	if (DrawPolylineOP::OnMouseLeftDClick(x, y)) return true;

	if (m_polyline.size() >= 3)
	{
		m_shapesImpl->insertShape(new PolygonShape(m_polyline));
		m_polyline.clear();
		m_currPos.setInvalid();
	}

	return false;
}

}