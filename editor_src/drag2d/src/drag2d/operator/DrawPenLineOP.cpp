#include "DrawPenLineOP.h"

#include "view/MultiShapesImpl.h"
#include "dataset/ChainShape.h"

namespace d2d
{

DrawPenLineOP::DrawPenLineOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl)
	: DrawPolylineOP(editPanel, false)
	, m_shapesImpl(shapesImpl)
{
}

bool DrawPenLineOP::onMouseLeftDClick(int x, int y)
{
	if (DrawPolylineOP::onMouseLeftDClick(x, y)) return true;

	m_shapesImpl->insertShape(new ChainShape(m_polyline, false));
	m_polyline.clear();
	m_currPos.setInvalid();

	return false;
}

} // d2d