#include "DrawPolylineOP.h"
#include "DrawLineUtility.h"

namespace libshape
{

DrawPolylineOP::DrawPolylineOP(d2d::EditPanel* editPanel, bool isClosed)
	: d2d::ZoomViewOP(editPanel, true, false)
{
	m_isClosed = isClosed;
	m_currPos.setInvalid();
}

bool DrawPolylineOP::onMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftDown(x, y)) return true;

	d2d::Vector pos = m_stage->transPosScreenToProject(x, y);
	if (DrawLineUtility::IsStraightOpen(m_polyline)) {
		pos = DrawLineUtility::FixPosTo8DirStraight(m_polyline.back(), pos);
	}
	m_polyline.push_back(pos);
	m_stage->Refresh();

	return false;
}

bool DrawPolylineOP::onMouseRightDown(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseRightDown(x, y)) return true;

	if (!m_polyline.empty())
	{
		m_polyline.pop_back();
		if (m_polyline.empty()) m_currPos.setInvalid();
		m_stage->Refresh();
	}

	return false;
}

bool DrawPolylineOP::onMouseMove(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseMove(x, y)) return true;

	if (m_polyline.empty()) return false;

	d2d::Vector pos = m_stage->transPosScreenToProject(x, y);
	if (DrawLineUtility::IsStraightOpen(m_polyline)) {
		pos = DrawLineUtility::FixPosTo8DirStraight(m_polyline.back(), pos);
	}
	m_currPos = pos;
	m_stage->Refresh();

	return false;
}

bool DrawPolylineOP::onMouseLeftDClick(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftDClick(x, y)) return true;

	if (m_isClosed)
		m_polyline.push_back(m_polyline.front());
	m_stage->Refresh();

	return false;
}

bool DrawPolylineOP::onDraw() const
{
	if (d2d::ZoomViewOP::onDraw()) return true;

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

bool DrawPolylineOP::clear()
{
	if (d2d::ZoomViewOP::clear()) return true;

	m_polyline.clear();
	m_currPos.setInvalid();

	return false;
}

bool DrawPolylineOP::ShouldFixPos() const
{
	return DrawLineUtility::IsStraightOpen(m_polyline);
}

}