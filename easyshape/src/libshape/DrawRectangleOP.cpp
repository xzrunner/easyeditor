#include "DrawRectangleOP.h"

#include <ee/EditPanelImpl.h>
#include <ee/panel_msg.h>
#include <ee/EE_RVG.h>

namespace eshape
{

DrawRectangleOP::DrawRectangleOP(wxWindow* wnd, ee::EditPanelImpl* stage, bool bOpenRightTap)
	: ee::ZoomViewOP(wnd, stage, true, bOpenRightTap)
	, m_first_pos_valid(false)
	, m_curr_pos_valid(false)
{
}

bool DrawRectangleOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	m_first_pos = m_stage->TransPosScrToProj(x, y);
	m_first_pos_valid = true;

	return false;
}

bool DrawRectangleOP::OnMouseLeftUp(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	m_first_pos_valid = m_curr_pos_valid = false;

	return false;
}

bool DrawRectangleOP::OnMouseDrag(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseDrag(x, y)) return true;

	if (m_first_pos_valid)
	{
		m_curr_pos = m_stage->TransPosScrToProj(x, y);
		m_curr_pos_valid = true;
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool DrawRectangleOP::OnDraw() const
{
	if (ee::ZoomViewOP::OnDraw()) return true;

	if (m_first_pos_valid && m_curr_pos_valid) {
		ee::RVG::Rect(m_first_pos, m_curr_pos, false);
	}

	return false;
}

bool DrawRectangleOP::Clear()
{
	if (ee::ZoomViewOP::Clear()) return true;

	m_first_pos_valid = m_curr_pos_valid = false;

	return false;
}

}