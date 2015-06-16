#include "DrawRectangleOP.h"

#include "render/PrimitiveDraw.h"

namespace d2d
{

DrawRectangleOP::DrawRectangleOP(EditPanel* editPanel, bool bOpenRightTap)
	: ZoomViewOP(editPanel, true, bOpenRightTap)
{
	m_firstPos.setInvalid();
	m_currPos.setInvalid();
}

bool DrawRectangleOP::OnMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	m_firstPos = m_stage->transPosScreenToProject(x, y);
	m_stage->Refresh();

	return false;
}

bool DrawRectangleOP::OnMouseLeftUp(int x, int y)
{
	if (ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	m_firstPos.setInvalid();
	m_currPos.setInvalid();
	m_stage->Refresh();

	return false;
}

bool DrawRectangleOP::OnMouseDrag(int x, int y)
{
	if (ZoomViewOP::OnMouseDrag(x, y)) return true;

	if (m_firstPos.isValid())
	{
		m_currPos = m_stage->transPosScreenToProject(x, y);
		m_stage->Refresh();
	}

	return false;
}

bool DrawRectangleOP::OnDraw() const
{
	if (ZoomViewOP::OnDraw()) return true;

	if (m_firstPos.isValid() && m_currPos.isValid()) {
		PrimitiveDraw::rect(m_firstPos, m_currPos, m_style);
	}

	return false;
}

bool DrawRectangleOP::Clear()
{
	if (ZoomViewOP::Clear()) return true;

	m_firstPos.setInvalid();
	m_currPos.setInvalid();

	return false;
}

} // d2d