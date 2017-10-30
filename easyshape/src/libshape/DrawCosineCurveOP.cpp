#include "DrawCosineCurveOP.h"
#include "CosineShape.h"

#include <ee/shape_msg.h>

namespace eshape
{

DrawCosineCurveOP::DrawCosineCurveOP(wxWindow* wnd, ee::EditPanelImpl* stage)
	: DrawPolylineOP(wnd, stage, false)
{
}

bool DrawCosineCurveOP::OnMouseLeftDClick(int x, int y)
{
	if (DrawPolylineOP::OnMouseLeftDClick(x, y)) return true;

	auto curve = std::make_unique<CosineShape>(m_polyline);
	ee::InsertShapeSJ::Instance()->Insert(std::move(curve));

	m_polyline.clear();
	m_curr_pos.MakeInvalid();

	return false;
}

}