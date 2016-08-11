#include "DrawRectangleOP.h"
#include "StageCanvas.h"
#include "EditPanelImpl.h"
#include "panel_msg.h"

#include <sprite2/S2_RVG.h>

namespace ee
{

DrawRectangleOP::DrawRectangleOP(wxWindow* wnd, EditPanelImpl* stage, bool bOpenRightTap)
	: ZoomViewOP(wnd, stage, true, bOpenRightTap)
{
	m_first_pos.MakeInvalid();
	m_last_pos.MakeInvalid();
}

bool DrawRectangleOP::OnMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	m_first_pos = m_stage->TransPosScrToProj(x, y);

	return false;
}

bool DrawRectangleOP::OnMouseLeftUp(int x, int y)
{
	if (ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	m_first_pos.MakeInvalid();
	m_last_pos.MakeInvalid();

	return false;
}

bool DrawRectangleOP::OnMouseDrag(int x, int y)
{
	if (ZoomViewOP::OnMouseDrag(x, y)) return true;

	if (m_first_pos.IsValid())
	{
		m_last_pos = m_stage->TransPosScrToProj(x, y);
		SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool DrawRectangleOP::OnDraw() const
{
	if (ZoomViewOP::OnDraw()) return true;

	if (m_first_pos.IsValid() && m_last_pos.IsValid()) {
		s2::RVG::Rect(m_first_pos, m_last_pos, false);
	}

	return false;
}

bool DrawRectangleOP::Clear()
{
	if (ZoomViewOP::Clear()) return true;

	m_first_pos.MakeInvalid();
	m_last_pos.MakeInvalid();

	return false;
}

}