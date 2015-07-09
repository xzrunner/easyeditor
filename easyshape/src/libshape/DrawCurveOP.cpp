#include "DrawCurveOP.h"
#include "DrawLineUtility.h"

namespace libshape
{

DrawCurveOP::DrawCurveOP(d2d::EditPanel* editPanel)
	: d2d::ZoomViewOP(editPanel, true)
	, m_startDraw(false)
	, m_straight_mode(false)
{
	m_cursor = wxCursor(wxCURSOR_PENCIL);

	m_firstPos.setInvalid();
}

bool DrawCurveOP::OnMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	m_firstPos.set(x, y);

	return false;
}

bool DrawCurveOP::OnMouseLeftUp(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	m_firstPos.setInvalid();
	m_startDraw = false;

	return false;
}

bool DrawCurveOP::OnMouseDrag(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseDrag(x, y)) return true;

	if (!m_startDraw && m_firstPos.isValid() && 
		(m_firstPos.x != x || m_firstPos.y != y)) {
		m_startDraw = true;
	}

	if (!m_startDraw) {
		return false;
	}

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	if (DrawLineUtility::IsStraightOpen(m_curve)) {
		pos = DrawLineUtility::FixPosTo8DirStraight(m_curve.back(), pos);
		m_curve.pop_back();
	}
	m_curve.push_back(pos);
	m_stage->Refresh();

	return false;
}

bool DrawCurveOP::OnDraw() const
{
	if (d2d::ZoomViewOP::OnDraw()) return true;

	if (!m_curve.empty()) {
		d2d::PrimitiveDraw::drawPolyline(m_curve, d2d::Colorf(0, 0, 0), false, 1);
	}

	return false;
}

bool DrawCurveOP::Clear()
{
	if (d2d::ZoomViewOP::Clear()) return true;

	m_curve.clear();
	m_firstPos.setInvalid();
	m_startDraw = false;

	return false;
}

}