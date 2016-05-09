#include "DrawRectangleOP.h"
#include "EE_RVG.h"
#include "StageCanvas.h"
#include "EditPanelImpl.h"
#include "panel_msg.h"

namespace ee
{

DrawRectangleOP::DrawRectangleOP(wxWindow* wnd, EditPanelImpl* stage, bool bOpenRightTap)
	: ZoomViewOP(wnd, stage, true, bOpenRightTap)
	, m_first_valid(false)
	, m_last_valid(false)
{
}

bool DrawRectangleOP::OnMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	m_first_pos = m_stage->TransPosScrToProj(x, y);
	m_first_valid = true;

	return false;
}

bool DrawRectangleOP::OnMouseLeftUp(int x, int y)
{
	if (ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	m_first_valid = false;
	m_last_valid = false;

	return false;
}

bool DrawRectangleOP::OnMouseDrag(int x, int y)
{
	if (ZoomViewOP::OnMouseDrag(x, y)) return true;

	if (m_first_valid)
	{
		m_last_pos = m_stage->TransPosScrToProj(x, y);
		m_last_valid = true;
		SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool DrawRectangleOP::OnDraw() const
{
	if (ZoomViewOP::OnDraw()) return true;

	if (m_first_valid && m_last_valid) {
		RVG::Rect(m_first_pos, m_last_pos, false);
	}

	return false;
}

bool DrawRectangleOP::Clear()
{
	if (ZoomViewOP::Clear()) return true;

	m_first_valid = m_last_valid = false;

	return false;
}

}