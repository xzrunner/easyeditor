#include "DrawPencilLineOP.h"
#include "ChainShape.h"

namespace libshape
{

DrawPencilLineOP::DrawPencilLineOP(wxWindow* wnd, d2d::EditPanelImpl* stage, 
								   d2d::OneFloatValue* simplify)
	: DrawCurveOP(wnd, stage)
	, m_simplify(simplify)
{
}

bool DrawPencilLineOP::OnMouseLeftUp(int x, int y)
{
	if (DrawCurveOP::OnMouseLeftUp(x, y)) return true;

	if (!m_curve.empty())
	{
		std::vector<d2d::Vector> simplified;
		d2d::DouglasPeucker::Do(m_curve, m_simplify->GetValue(), simplified);
		d2d::InsertShapeSJ::Instance()->Insert(new ChainShape(simplified, false));
		Clear();

		d2d::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

}