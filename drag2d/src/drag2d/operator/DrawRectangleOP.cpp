#include "DrawRectangleOP.h"

#include "render/PrimitiveDraw.h"
#include "view/IStageCanvas.h"
#include "view/EditPanelImpl.h"

namespace d2d
{

DrawRectangleOP::DrawRectangleOP(wxWindow* wnd, d2d::EditPanelImpl* stage, bool bOpenRightTap)
	: ZoomViewOP(wnd, stage, true, bOpenRightTap)
{
	m_first_pos.setInvalid();
	m_last_pos.setInvalid();
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

	m_first_pos.setInvalid();
	m_last_pos.setInvalid();

	return false;
}

bool DrawRectangleOP::OnMouseDrag(int x, int y)
{
	if (ZoomViewOP::OnMouseDrag(x, y)) return true;

	if (m_first_pos.isValid())
	{
		m_last_pos = m_stage->TransPosScrToProj(x, y);
		m_stage->SetCanvasDirty();
	}

	return false;
}

bool DrawRectangleOP::OnDraw() const
{
	if (ZoomViewOP::OnDraw()) return true;

	if (m_first_pos.isValid() && m_last_pos.isValid()) {
		PrimitiveDraw::rect(m_first_pos, m_last_pos, m_style);
	}

	return false;
}

bool DrawRectangleOP::Clear()
{
	if (ZoomViewOP::Clear()) return true;

	m_first_pos.setInvalid();
	m_last_pos.setInvalid();

	return false;
}

} // d2d