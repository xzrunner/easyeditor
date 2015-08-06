#include "DrawPolylineOP.h"
#include "DrawLineUtility.h"

namespace libshape
{

DrawPolylineOP::DrawPolylineOP(wxWindow* wnd, d2d::EditPanelImpl* stage, bool isClosed)
	: d2d::ZoomViewOP(wnd, stage, true, false)
{
	m_cursor = wxCursor(wxCURSOR_RIGHT_ARROW);

	m_isClosed = isClosed;
	m_currPos.setInvalid();
}

bool DrawPolylineOP::OnMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	if (DrawLineUtility::IsStraightOpen(m_polyline, m_stage->GetKeyState())) {
		pos = DrawLineUtility::FixPosTo8DirStraight(m_polyline.back(), pos);
	}
	m_polyline.push_back(pos);
	m_stage->SetCanvasDirty();

	return false;
}

bool DrawPolylineOP::OnMouseRightDown(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseRightDown(x, y)) return true;

	if (!m_polyline.empty())
	{
		m_polyline.pop_back();
		if (m_polyline.empty()) m_currPos.setInvalid();
		m_stage->SetCanvasDirty();
	}

	return false;
}

bool DrawPolylineOP::OnMouseMove(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseMove(x, y)) return true;

	if (m_polyline.empty()) return false;

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	if (DrawLineUtility::IsStraightOpen(m_polyline, m_stage->GetKeyState())) {
		pos = DrawLineUtility::FixPosTo8DirStraight(m_polyline.back(), pos);
	}
	m_currPos = pos;
	m_stage->SetCanvasDirty();

	return false;
}

bool DrawPolylineOP::OnMouseLeftDClick(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftDClick(x, y)) return true;

	if (m_isClosed)
		m_polyline.push_back(m_polyline.front());
	m_stage->SetCanvasDirty();

	return false;
}

bool DrawPolylineOP::OnDraw() const
{
	if (d2d::ZoomViewOP::OnDraw()) return true;

	if (!m_polyline.empty())
	{
		if (m_currPos.isValid())
		{
			m_polyline.push_back(m_currPos);
			d2d::PrimitiveDraw::drawPolyline(m_polyline, d2d::Colorf(0, 0, 0), false, 2);
			m_polyline.pop_back();
		}
		else
		{
			d2d::PrimitiveDraw::drawPolyline(m_polyline, d2d::Colorf(0, 0, 0), false, 2);
		}
	}

	return false;
}

bool DrawPolylineOP::Clear()
{
	if (d2d::ZoomViewOP::Clear()) return true;

	m_polyline.clear();
	m_currPos.setInvalid();

	return false;
}

bool DrawPolylineOP::ShouldFixPos() const
{
	return DrawLineUtility::IsStraightOpen(m_polyline, m_stage->GetKeyState());
}

}