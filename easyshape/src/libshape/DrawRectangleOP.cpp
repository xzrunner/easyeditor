#include "DrawRectangleOP.h"

namespace libshape
{

DrawRectangleOP::DrawRectangleOP(wxWindow* wnd, d2d::EditPanelImpl* stage, bool bOpenRightTap)
	: d2d::ZoomViewOP(wnd, stage, true, bOpenRightTap)
{
	m_firstPos.SetInvalid();
	m_currPos.SetInvalid();
}

bool DrawRectangleOP::OnMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	m_firstPos = m_stage->TransPosScrToProj(x, y);


	return false;
}

bool DrawRectangleOP::OnMouseLeftUp(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	m_firstPos.SetInvalid();
	m_currPos.SetInvalid();


	return false;
}

bool DrawRectangleOP::OnMouseDrag(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseDrag(x, y)) return true;

	if (m_firstPos.IsValid())
	{
		m_currPos = m_stage->TransPosScrToProj(x, y);
		d2d::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool DrawRectangleOP::OnDraw() const
{
	if (d2d::ZoomViewOP::OnDraw()) return true;

	if (m_firstPos.IsValid() && m_currPos.IsValid()) {
		d2d::PrimitiveDraw::DrawRect(m_firstPos, m_currPos, m_style);
	}

	return false;
}

bool DrawRectangleOP::Clear()
{
	if (d2d::ZoomViewOP::Clear()) return true;

	m_firstPos.SetInvalid();
	m_currPos.SetInvalid();

	return false;
}

}