#include "DrawSelectRectOP.h"
#include "EE_RVG.h"
#include "color_config.h"
#include "StageCanvas.h"
#include "EditPanelImpl.h"
#include "panel_msg.h"

namespace ee
{

DrawSelectRectOP::DrawSelectRectOP(wxWindow* wnd, EditPanelImpl* stage, 
								   bool bOpenRightTap)
	: ZoomViewOP(wnd, stage, true, bOpenRightTap)
{
	m_first_valid = m_last_valid = false;
}

bool DrawSelectRectOP::OnMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	m_first_pos = m_stage->TransPosScrToProj(x, y);
	m_first_valid = true;

	return false;
}

bool DrawSelectRectOP::OnMouseLeftUp(int x, int y)
{
	if (ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	m_first_valid = m_last_valid = false;
	SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool DrawSelectRectOP::OnMouseDrag(int x, int y)
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

bool DrawSelectRectOP::OnDraw() const
{
	if (ZoomViewOP::OnDraw()) return true;

	if (!m_first_valid || !m_last_valid) {
		return false;
	}

	RVG::LineWidth(2);
	if (m_last_pos.x > m_first_pos.x)
	{
		RVG::Color(LIGHT_GREEN);
		RVG::Rect(m_first_pos, m_last_pos, true);
		RVG::Color(SELECT_RED);
		RVG::Rect(m_first_pos, m_last_pos, false);
	}
	else
	{
		RVG::Color(LIGHT_BLUE);
		RVG::Rect(m_first_pos, m_last_pos, true);
		RVG::Color(SELECT_RED);
		RVG::Rect(m_first_pos, m_last_pos, false);
	}

	return false;
}

bool DrawSelectRectOP::Clear()
{
	if (ZoomViewOP::Clear()) return true;

	m_first_valid = m_last_valid = false;

	return false;
}

}