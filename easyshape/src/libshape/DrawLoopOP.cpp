#include "DrawLoopOP.h"
#include "ChainShape.h"

namespace libshape
{

DrawLoopOP::DrawLoopOP(d2d::EditPanel* editPanel, 
					   d2d::MultiShapesImpl* shapesImpl)
	: DrawPolylineOP(editPanel, false)
	, m_shapesImpl(shapesImpl)
{
}

bool DrawLoopOP::onMouseLeftDClick(int x, int y)
{
	if (DrawPolylineOP::onMouseLeftDClick(x, y)) return true;

	d2d::IShape* shape = new ChainShape(m_polyline, true);
	m_shapesImpl->insertShape(shape);
	shape->Release();

	m_polyline.clear();
	m_currPos.setInvalid();

	return false;
}

}