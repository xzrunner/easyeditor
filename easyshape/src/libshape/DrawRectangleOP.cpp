#include "DrawRectangleOP.h"

namespace eshape
{

DrawRectangleOP::DrawRectangleOP(wxWindow* wnd, ee::EditPanelImpl* stage, bool bOpenRightTap)
	: ee::ZoomViewOP(wnd, stage, true, bOpenRightTap)
{
	m_firstPos.SetInvalid();
	m_currPos.SetInvalid();
}

bool DrawRectangleOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	m_firstPos = m_stage->TransPosScrToProj(x, y);


	return false;
}

bool DrawRectangleOP::OnMouseLeftUp(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	m_firstPos.SetInvalid();
	m_currPos.SetInvalid();


	return false;
}

bool DrawRectangleOP::OnMouseDrag(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseDrag(x, y)) return true;

	if (m_firstPos.IsValid())
	{
		m_currPos = m_stage->TransPosScrToProj(x, y);
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool DrawRectangleOP::OnDraw() const
{
	if (ee::ZoomViewOP::OnDraw()) return true;

	if (m_firstPos.IsValid() && m_currPos.IsValid()) {
		ee::PrimitiveDraw::DrawRect(m_firstPos, m_currPos, m_style);
	}

	return false;
}

bool DrawRectangleOP::Clear()
{
	if (ee::ZoomViewOP::Clear()) return true;

	m_firstPos.SetInvalid();
	m_currPos.SetInvalid();

	return false;
}

}