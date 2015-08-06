#include "DrawPencilLineOP.h"
#include "ChainShape.h"

namespace libshape
{

DrawPencilLineOP::DrawPencilLineOP(wxWindow* wnd, d2d::EditPanelImpl* stage, 
								   d2d::MultiShapesImpl* shapesImpl,
								   d2d::OneFloatValue* simplify)
	: DrawCurveOP(wnd, stage)
	, m_shapesImpl(shapesImpl)
	, m_simplify(simplify)
{
}

bool DrawPencilLineOP::OnMouseLeftUp(int x, int y)
{
	if (DrawCurveOP::OnMouseLeftUp(x, y)) return true;

	if (!m_curve.empty())
	{
		std::vector<d2d::Vector> simplified;
		d2d::DouglasPeucker::implement(m_curve, m_simplify->GetValue(), simplified);
		m_shapesImpl->InsertShape(new ChainShape(simplified, false));
		Clear();

		m_stage->SetCanvasDirty();
	}

	return false;
}

}