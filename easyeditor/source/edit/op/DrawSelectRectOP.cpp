#include "DrawSelectRectOP.h"
#include "PrimitiveDraw.h"
#include "style_config.h"
#include "StageCanvas.h"
#include "EditPanelImpl.h"
#include "panel_msg.h"

namespace ee
{

DrawSelectRectOP::DrawSelectRectOP(wxWindow* wnd, EditPanelImpl* stage, 
								   bool bOpenRightTap)
	: ZoomViewOP(wnd, stage, true, bOpenRightTap)
{
	m_first_pos.SetInvalid();
	m_last_pos.SetInvalid();
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

	m_first_pos.SetInvalid();
	m_last_pos.SetInvalid();

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

	if (m_last_pos.x > m_first_pos.x)
	{
		PrimitiveDraw::DrawRect(m_first_pos, m_last_pos, SELECT_ALL);
		PrimitiveDraw::DrawRect(m_first_pos, m_last_pos, SELECT_BOUND);
	}
	else
	{
		PrimitiveDraw::DrawRect(m_first_pos, m_last_pos, SELECT_PART);
		PrimitiveDraw::DrawRect(m_first_pos, m_last_pos, SELECT_BOUND);
	}

	return false;
}

bool DrawSelectRectOP::Clear()
{
	if (ZoomViewOP::Clear()) return true;

	m_first_pos.SetInvalid();
	m_last_pos.SetInvalid();

	return false;
}

}