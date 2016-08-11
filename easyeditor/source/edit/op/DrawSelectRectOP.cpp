#include "DrawSelectRectOP.h"
#include "color_config.h"
#include "StageCanvas.h"
#include "EditPanelImpl.h"
#include "panel_msg.h"

#include <sprite2/S2_RVG.h>

namespace ee
{

DrawSelectRectOP::DrawSelectRectOP(wxWindow* wnd, EditPanelImpl* stage, 
								   bool bOpenRightTap)
	: ZoomViewOP(wnd, stage, true, bOpenRightTap)
{
	m_first_pos.MakeInvalid();
	m_last_pos.MakeInvalid();
}

bool DrawSelectRectOP::OnMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	m_first_pos = m_stage->TransPosScrToProj(x, y);

	return false;
}

bool DrawSelectRectOP::OnMouseLeftUp(int x, int y)
{
	if (ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	m_first_pos.MakeInvalid();
	m_last_pos.MakeInvalid();

	SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool DrawSelectRectOP::OnMouseDrag(int x, int y)
{
	if (ZoomViewOP::OnMouseDrag(x, y)) return true;

	if (m_first_pos.IsValid())
	{
		m_last_pos = m_stage->TransPosScrToProj(x, y);
		SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool DrawSelectRectOP::OnDraw() const
{
	if (ZoomViewOP::OnDraw()) return true;

	if (!m_first_pos.IsValid() || !m_last_pos.IsValid()) {
		return false;
	}

	s2::RVG::LineWidth(2);
	if (m_last_pos.x > m_first_pos.x)
	{
		s2::RVG::SetColor(LIGHT_GREEN);
		s2::RVG::Rect(m_first_pos, m_last_pos, true);
		s2::RVG::SetColor(SELECT_RED);
		s2::RVG::Rect(m_first_pos, m_last_pos, false);
	}
	else
	{
		s2::RVG::SetColor(LIGHT_BLUE);
		s2::RVG::Rect(m_first_pos, m_last_pos, true);
		s2::RVG::SetColor(SELECT_RED);
		s2::RVG::Rect(m_first_pos, m_last_pos, false);
	}

	return false;
}

bool DrawSelectRectOP::Clear()
{
	if (ZoomViewOP::Clear()) return true;

	m_first_pos.MakeInvalid();
	m_last_pos.MakeInvalid();

	return false;
}

}