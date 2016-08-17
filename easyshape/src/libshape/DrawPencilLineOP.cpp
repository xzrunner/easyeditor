#include "DrawPencilLineOP.h"
#include "ChainShape.h"

#include <ee/OneFloatValue.h>
#include <ee/shape_msg.h>
#include <ee/panel_msg.h>

#include <SM_DouglasPeucker.h>

namespace eshape
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
		std::vector<sm::vec2> simplified;
		sm::douglas_peucker(m_curve, m_simplify->GetValue(), simplified);
		ChainShape* chain = new ChainShape(simplified, false);
		ee::InsertShapeSJ::Instance()->Insert(chain);
		chain->RemoveReference();
		Clear();

		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

}