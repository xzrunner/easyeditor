#include "DrawCurveOP.h"
#include "DrawLineUtility.h"

namespace libshape
{

DrawCurveOP::DrawCurveOP(wxWindow* wnd, d2d::EditPanelImpl* stage)
	: d2d::ZoomViewOP(wnd, stage, true)
	, m_startDraw(false)
	, m_straight_mode(false)
{
	m_cursor = wxCursor(wxCURSOR_PENCIL);

	m_firstPos.SetInvalid();
}

bool DrawCurveOP::OnMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	m_firstPos.Set(x, y);

	return false;
}

bool DrawCurveOP::OnMouseLeftUp(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	m_firstPos.SetInvalid();
	m_startDraw = false;

	return false;
}

bool DrawCurveOP::OnMouseDrag(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseDrag(x, y)) return true;

	if (!m_startDraw && m_firstPos.IsValid() && 
		(m_firstPos.x != x || m_firstPos.y != y)) {
		m_startDraw = true;
	}

	if (!m_startDraw) {
		return false;
	}

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	if (DrawLineUtility::IsStraightOpen(m_curve, m_stage->GetKeyState())) {
		pos = DrawLineUtility::FixPosTo8DirStraight(m_curve.back(), pos);
		m_curve.pop_back();
	}
	m_curve.push_back(pos);
	d2d::SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool DrawCurveOP::OnDraw() const
{
	if (d2d::ZoomViewOP::OnDraw()) return true;

	if (!m_curve.empty()) {
		d2d::PrimitiveDraw::DrawPolyline(m_curve, d2d::Colorf(0, 0, 0), false, 1);
	}

	return false;
}

bool DrawCurveOP::Clear()
{
	if (d2d::ZoomViewOP::Clear()) return true;

	m_curve.clear();
	m_firstPos.SetInvalid();
	m_startDraw = false;

	return false;
}

}