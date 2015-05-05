#include "DrawPencilPolygonOP.h"
#include "PolygonShape.h"

namespace libshape
{

DrawPencilPolygonOP::DrawPencilPolygonOP(d2d::EditPanel* editPanel, 
										 d2d::MultiShapesImpl* shapesImpl,
										 d2d::OneFloatValue* simplify)
	: DrawCurveOP(editPanel)
	, m_shapesImpl(shapesImpl)
	, m_simplify(simplify)
{
}

bool DrawPencilPolygonOP::onMouseLeftUp(int x, int y)
{
	if (DrawCurveOP::onMouseLeftUp(x, y)) return true;

	if (!m_curve.empty())
	{
		std::vector<d2d::Vector> simplified;
		d2d::DouglasPeucker::implement(m_curve, m_simplify->GetValue(), simplified);
		m_shapesImpl->insertShape(new PolygonShape(simplified));
		clear();

		m_editPanel->Refresh();
	}

	return false;
}

}