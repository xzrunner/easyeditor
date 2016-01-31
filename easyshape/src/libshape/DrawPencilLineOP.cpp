#include "DrawPencilLineOP.h"
#include "ChainShape.h"

namespace libshape
{

DrawPencilLineOP::DrawPencilLineOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
								   ee::OneFloatValue* simplify)
	: DrawCurveOP(wnd, stage)
	, m_simplify(simplify)
{
}

bool DrawPencilLineOP::OnMouseLeftUp(int x, int y)
{
	if (DrawCurveOP::OnMouseLeftUp(x, y)) return true;

	if (!m_curve.empty())
	{
		std::vector<ee::Vector> simplified;
		ee::DouglasPeucker::Do(m_curve, m_simplify->GetValue(), simplified);
		ee::InsertShapeSJ::Instance()->Insert(new ChainShape(simplified, false));
		Clear();

		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

}