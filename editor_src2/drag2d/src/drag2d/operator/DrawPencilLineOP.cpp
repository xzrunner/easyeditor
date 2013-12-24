#include "DrawPencilLineOP.h"

#include "algorithms/DouglasPeucker.h"
#include "component/DrawPencilLineCMPT.h"
#include "view/MultiShapesImpl.h"
#include "dataset/ChainShape.h"

namespace d2d
{

DrawPencilLineOP::DrawPencilLineOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl,
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
		std::vector<Vector> simplified;
		DouglasPeucker::implement(m_curve, m_cmpt->getSimplifyThreshold(), simplified);
		m_shapesImpl->insertShape(new ChainShape(simplified, false));
		clear();
	}

	return false;
}

} // d2d