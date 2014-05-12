#include "DrawCosineCurveOP.h"
#include "CosineShape.h"

namespace libshape
{

DrawCosineCurveOP::DrawCosineCurveOP(d2d::EditPanel* editPanel, 
									 d2d::MultiShapesImpl* shapesImpl)
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

}