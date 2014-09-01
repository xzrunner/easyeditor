#include "DrawPolylineOP.h"

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

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	if (shouldFixMousePos())
		fixPosTo45Degree(pos);
	m_polyline.push_back(pos);
	m_editPanel->Refresh();

	return false;
}

bool DrawPolylineOP::onMouseRightDown(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseRightDown(x, y)) return true;

	if (!m_polyline.empty())
	{
		m_polyline.pop_back();
		if (m_polyline.empty()) m_currPos.setInvalid();
		m_editPanel->Refresh();
	}

	return false;
}

bool DrawPolylineOP::onMouseMove(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseMove(x, y)) return true;

	if (m_polyline.empty()) return false;

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	if (shouldFixMousePos())
		fixPosTo45Degree(pos);
	m_currPos = pos;
	m_editPanel->Refresh();

	return false;
}

bool DrawPolylineOP::onMouseLeftDClick(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftDClick(x, y)) return true;

	if (m_isClosed)
		m_polyline.push_back(m_polyline.front());
	m_editPanel->Refresh();

	return false;
}

bool DrawPolylineOP::onDraw(const d2d::Screen& scr) const
{
	if (d2d::ZoomViewOP::onDraw(scr)) return true;

	if (!m_polyline.empty())
	{
		if (m_currPos.isValid())
		{
			m_polyline.push_back(m_currPos);
			d2d::PrimitiveDraw::drawPolyline(scr, m_polyline, d2d::Colorf(0, 0, 0), false, 2);
			m_polyline.pop_back();
		}
		else
		{
			d2d::PrimitiveDraw::drawPolyline(scr, m_polyline, d2d::Colorf(0, 0, 0), false, 2);
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

bool DrawPolylineOP::shouldFixMousePos() const
{
	return !m_polyline.empty() && wxGetKeyState(WXK_SHIFT);
}

void DrawPolylineOP::fixPosTo45Degree(d2d::Vector& pos) const
{
	d2d::Vector last = m_polyline.back();

	float nearest;
	d2d::Vector fixed = pos;

	const float dx = fabs(pos.x - last.x);
	nearest = dx;
	fixed.set(last.x, pos.y);

	const float dy = fabs(pos.y - last.y);
	if (dy < nearest)
	{
		nearest = dy;
		fixed.set(pos.x, last.y);
	}

	d2d::Vector other(last.x + 1, last.y - 1);
	const float dxyDown = d2d::Math::getDisPointToStraightLine(pos, last, other);
	if (dxyDown < nearest)
	{
		nearest = dxyDown;
		d2d::Math::getFootOfPerpendicular(last, other, pos, &fixed);
	}

	other.set(last.x + 1, last.y + 1);
	const float dxyUp = d2d::Math::getDisPointToStraightLine(pos, last, other);
	if (dxyUp < nearest)
	{
		nearest = dxyUp;
		d2d::Math::getFootOfPerpendicular(last, other, pos, &fixed);
	}

	pos = fixed;
}

}