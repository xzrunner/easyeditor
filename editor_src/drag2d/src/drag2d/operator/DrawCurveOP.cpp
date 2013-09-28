#include "DrawCurveOP.h"

#include "render/PrimitiveDraw.h"

namespace d2d
{

DrawCurveOP::DrawCurveOP(EditPanel* editPanel)
	: ZoomViewOP(editPanel, true)
	, m_startDraw(false)
{
	m_firstPos.setInvalid();
}

bool DrawCurveOP::onMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::onMouseLeftDown(x, y)) return true;

	m_firstPos.set(x, y);

	return false;
}

bool DrawCurveOP::onMouseLeftUp(int x, int y)
{
	if (ZoomViewOP::onMouseLeftUp(x, y)) return true;

	m_firstPos.setInvalid();
	m_startDraw = false;

	return false;
}

bool DrawCurveOP::onMouseDrag(int x, int y)
{
	if (ZoomViewOP::onMouseDrag(x, y)) return true;

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

bool DrawCurveOP::onDraw() const
{
	if (ZoomViewOP::onDraw()) return true;

	if (!m_curve.empty())
		PrimitiveDraw::drawPolyline(m_curve, Colorf(0, 0, 0), false, 1);

	return false;
}

bool DrawCurveOP::clear()
{
	if (ZoomViewOP::clear()) return true;

	m_curve.clear();
	m_firstPos.setInvalid();
	m_startDraw = false;

	return false;
}

} // d2d