#include "DrawPolygonEdgeOP.h"

#include "view/MultiShapesImpl.h"
#include "dataset/PolygonShape.h"

namespace d2d
{

DrawPolygonEdgeOP::DrawPolygonEdgeOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl)
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

} // d2d