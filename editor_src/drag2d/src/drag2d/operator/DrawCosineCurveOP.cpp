#include "DrawCosineCurveOP.h"

#include "view/MultiShapesImpl.h"
#include "dataset/CosineShape.h"

namespace d2d
{

DrawCosineCurveOP::DrawCosineCurveOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl)
	: DrawPolylineOP(editPanel, false)
	, m_shapesImpl(shapesImpl)
{
}

bool DrawCosineCurveOP::onMouseLeftDClick(int x, int y)
{
	if (DrawPolylineOP::onMouseLeftDClick(x, y)) return true;

	CosineShape* curve = new CosineShape(m_polyline);
	curve->setMidPoints();

	m_shapesImpl->insertShape(curve);
	m_polyline.clear();
	m_currPos.setInvalid();

	return false;
}

} // d2d