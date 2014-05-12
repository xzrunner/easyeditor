#include "DrawPencilLineOP.h"
#include "DrawPencilLineCMPT.h"
#include "ChainShape.h"

namespace libshape
{

DrawPencilLineOP::DrawPencilLineOP(d2d::EditPanel* editPanel, 
								   d2d::MultiShapesImpl* shapesImpl,
								   DrawPencilLineCMPT* cmpt)
	: DrawCurveOP(editPanel)
	, m_shapesImpl(shapesImpl)
	, m_cmpt(cmpt)
{
}

bool DrawPencilLineOP::onMouseLeftUp(int x, int y)
{
	if (DrawCurveOP::onMouseLeftUp(x, y)) return true;

	if (!m_curve.empty())
	{
		std::vector<d2d::Vector> simplified;
		d2d::DouglasPeucker::implement(m_curve, m_cmpt->getSimplifyThreshold(), simplified);
		m_shapesImpl->insertShape(new ChainShape(simplified, false));
		clear();
	}

	return false;
}

}