#include "DrawPolygonEdgeOP.h"
#include "PolygonShape.h"

namespace libshape
{

DrawPolygonEdgeOP::DrawPolygonEdgeOP(d2d::EditPanel* editPanel, 
									 d2d::MultiShapesImpl* shapesImpl)
	: DrawPolylineOP(editPanel, false)
	, m_shapesImpl(shapesImpl)
{
}

bool DrawPolygonEdgeOP::onMouseLeftDClick(int x, int y)
{
	if (DrawPolylineOP::onMouseLeftDClick(x, y)) return true;

	if (m_polyline.size() >= 3)
	{
		m_shapesImpl->insertShape(new PolygonShape(m_polyline));
		m_polyline.clear();
		m_currPos.setInvalid();
	}

	return false;
}

}