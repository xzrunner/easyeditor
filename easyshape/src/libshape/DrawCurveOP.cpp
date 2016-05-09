#include "DrawCurveOP.h"
#include "DrawLineUtility.h"

#include <ee/EditPanelImpl.h>
#include <ee/panel_msg.h>
#include <ee/EE_RVG.h>
#include <ee/Color.h>

namespace eshape
{

DrawCurveOP::DrawCurveOP(wxWindow* wnd, ee::EditPanelImpl* stage)
	: ee::ZoomViewOP(wnd, stage, true)
	, m_start_draw(false)
	, m_straight_mode(false)
	, m_first_pos_valid(false)
{
	m_cursor = wxCursor(wxCURSOR_PENCIL);
}

bool DrawCurveOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	m_first_pos.Set(x, y);
	m_first_pos_valid = true;

	return false;
}

bool DrawCurveOP::OnMouseLeftUp(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	m_first_pos_valid = false;
	m_start_draw = false;

	return false;
}

bool DrawCurveOP::OnMouseDrag(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseDrag(x, y)) return true;

	if (!m_start_draw && m_first_pos_valid && 
		(m_first_pos.x != x || m_first_pos.y != y)) {
		m_start_draw = true;
	}

	if (!m_start_draw) {
		return false;
	}

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	if (DrawLineUtility::IsStraightOpen(m_curve, m_stage->GetKeyState())) {
		pos = DrawLineUtility::FixPosTo8DirStraight(m_curve.back(), pos);
		m_curve.pop_back();
	}
	m_curve.push_back(pos);
	ee::SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool DrawCurveOP::OnDraw() const
{
	if (ee::ZoomViewOP::OnDraw()) return true;

	if (!m_curve.empty()) {
		ee::RVG::Color(ee::Colorf(0, 0, 0));
		ee::RVG::LineWidth(1);
		ee::RVG::Polyline(m_curve, false);
	}

	return false;
}

bool DrawCurveOP::Clear()
{
	if (ee::ZoomViewOP::Clear()) return true;

	m_curve.clear();
	m_first_pos_valid = false;
	m_start_draw = false;

	return false;
}

}