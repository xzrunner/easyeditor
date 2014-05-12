#include "DrawRectangleOP.h"

namespace libshape
{

DrawRectangleOP::DrawRectangleOP(d2d::EditPanel* editPanel, bool bOpenRightTap)
	: d2d::ZoomViewOP(editPanel, true, bOpenRightTap)
{
	m_firstPos.setInvalid();
	m_currPos.setInvalid();
}

bool DrawRectangleOP::onMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftDown(x, y)) return true;

	m_firstPos = m_editPanel->transPosScreenToProject(x, y);
	m_editPanel->Refresh();

	return false;
}

bool DrawRectangleOP::onMouseLeftUp(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftUp(x, y)) return true;

	m_firstPos.setInvalid();
	m_currPos.setInvalid();
	m_editPanel->Refresh();

	return false;
}

bool DrawRectangleOP::onMouseDrag(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseDrag(x, y)) return true;

	if (m_firstPos.isValid())
	{
		m_currPos = m_editPanel->transPosScreenToProject(x, y);
		m_editPanel->Refresh();
	}

	return false;
}

bool DrawRectangleOP::onDraw() const
{
	if (d2d::ZoomViewOP::onDraw()) return true;

	if (m_firstPos.isValid() && m_currPos.isValid())
		d2d::PrimitiveDraw::rect(m_firstPos, m_currPos, m_style);

	return false;
}

bool DrawRectangleOP::clear()
{
	if (d2d::ZoomViewOP::clear()) return true;

	m_firstPos.setInvalid();
	m_currPos.setInvalid();

	return false;
}

}