#include "DrawPolylineOP.h"
#include "DrawLineUtility.h"

#include <ee/EditPanelImpl.h>
#include <ee/panel_msg.h>
#include <ee/EE_RVG.h>
#include <ee/Color.h>

namespace eshape
{

DrawPolylineOP::DrawPolylineOP(wxWindow* wnd, ee::EditPanelImpl* stage, bool isClosed)
	: ee::ZoomViewOP(wnd, stage, true, false)
{
	m_cursor = wxCursor(wxCURSOR_RIGHT_ARROW);

	m_isClosed = isClosed;
	m_curr_pos.SetInvalid();
}

bool DrawPolylineOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	ee::Vector pos = m_stage->TransPosScrToProj(x, y);
	if (DrawLineUtility::IsStraightOpen(m_polyline, m_stage->GetKeyState())) {
		pos = DrawLineUtility::FixPosTo8DirStraight(m_polyline.back(), pos);
	}
	m_polyline.push_back(pos);
	ee::SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool DrawPolylineOP::OnMouseRightDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseRightDown(x, y)) return true;

	if (!m_polyline.empty())
	{
		m_polyline.pop_back();
		if (m_polyline.empty()) m_curr_pos.SetInvalid();
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool DrawPolylineOP::OnMouseMove(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseMove(x, y)) return true;

	if (m_polyline.empty()) return false;

	ee::Vector pos = m_stage->TransPosScrToProj(x, y);
	if (DrawLineUtility::IsStraightOpen(m_polyline, m_stage->GetKeyState())) {
		pos = DrawLineUtility::FixPosTo8DirStraight(m_polyline.back(), pos);
	}
	m_curr_pos = pos;
	ee::SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool DrawPolylineOP::OnMouseLeftDClick(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDClick(x, y)) return true;

	if (m_isClosed)
		m_polyline.push_back(m_polyline.front());
	ee::SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool DrawPolylineOP::OnDraw() const
{
	if (ee::ZoomViewOP::OnDraw()) return true;

	if (!m_polyline.empty())
	{
		if (m_curr_pos.IsValid())
		{
			m_polyline.push_back(m_curr_pos);
			ee::RVG::Polyline(m_polyline, ee::Colorf(0, 0, 0), false, 2);
			m_polyline.pop_back();
		}
		else
		{
			ee::RVG::Polyline(m_polyline, ee::Colorf(0, 0, 0), false, 2);
		}
	}

	return false;
}

bool DrawPolylineOP::Clear()
{
	if (ee::ZoomViewOP::Clear()) return true;

	m_polyline.clear();
	m_curr_pos.SetInvalid();

	return false;
}

bool DrawPolylineOP::ShouldFixPos() const
{
	return DrawLineUtility::IsStraightOpen(m_polyline, m_stage->GetKeyState());
}

}