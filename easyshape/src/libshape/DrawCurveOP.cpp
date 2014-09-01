#include "DrawCurveOP.h"

namespace libshape
{

DrawCurveOP::DrawCurveOP(d2d::EditPanel* editPanel)
	: d2d::ZoomViewOP(editPanel, true)
	, m_startDraw(false)
{
	m_firstPos.setInvalid();
}

bool DrawCurveOP::onMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftDown(x, y)) return true;

	m_firstPos.set(x, y);

	return false;
}

bool DrawCurveOP::onMouseLeftUp(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftUp(x, y)) return true;

	m_firstPos.setInvalid();
	m_startDraw = false;

	return false;
}

bool DrawCurveOP::onMouseDrag(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseDrag(x, y)) return true;

	if (!m_startDraw && m_firstPos.isValid() && 
		(m_firstPos.x != x || m_firstPos.y != y))
		m_startDraw = true;

	if (m_startDraw)
	{
		m_curve.push_back(m_editPanel->transPosScreenToProject(x, y));
		m_editPanel->Refresh();
	}

	return false;
}

bool DrawCurveOP::onDraw(const d2d::Screen& scr) const
{
	if (d2d::ZoomViewOP::onDraw(scr)) return true;

	if (!m_curve.empty()) {
		d2d::PrimitiveDraw::drawPolyline(scr, m_curve, d2d::Colorf(0, 0, 0), false, 1);
	}

	return false;
}

bool DrawCurveOP::clear()
{
	if (d2d::ZoomViewOP::clear()) return true;

	m_curve.clear();
	m_firstPos.setInvalid();
	m_startDraw = false;

	return false;
}

}