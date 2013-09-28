#include "DrawPolylineOP.h"

#include "common/Math.h"
#include "view/Camera.h"
#include "render/PrimitiveDraw.h"

namespace d2d
{

DrawPolylineOP::DrawPolylineOP(EditPanel* editPanel, bool isClosed)
	: ZoomViewOP(editPanel, true)
{
	m_isClosed = isClosed;
	m_currPos.setInvalid();
}

bool DrawPolylineOP::onMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::onMouseLeftDown(x, y)) return true;

	Vector pos = m_editPanel->transPosScreenToProject(x, y);
	if (shouldFixMousePos())
		fixPosTo45Degree(pos);
	m_polyline.push_back(pos);
	m_editPanel->Refresh();

	return false;
}

bool DrawPolylineOP::onMouseRightDown(int x, int y)
{
	if (ZoomViewOP::onMouseRightDown(x, y)) return true;

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
	if (ZoomViewOP::onMouseMove(x, y)) return true;

	if (m_polyline.empty()) return false;

	Vector pos = m_editPanel->transPosScreenToProject(x, y);
	if (shouldFixMousePos())
		fixPosTo45Degree(pos);
	m_currPos = pos;
	m_editPanel->Refresh();

	return false;
}

bool DrawPolylineOP::onMouseLeftDClick(int x, int y)
{
	if (ZoomViewOP::onMouseLeftDClick(x, y)) return true;

	if (m_isClosed)
		m_polyline.push_back(m_polyline.front());
	m_editPanel->Refresh();

	return false;
}

bool DrawPolylineOP::onDraw() const
{
	if (ZoomViewOP::onDraw()) return true;

	if (!m_polyline.empty())
	{
		if (m_currPos.isValid())
		{
			m_polyline.push_back(m_currPos);
			PrimitiveDraw::drawPolyline(m_polyline, Colorf(0, 0, 0), false, 2);
			m_polyline.pop_back();
		}
		else
			PrimitiveDraw::drawPolyline(m_polyline, Colorf(0, 0, 0), false, 2);
	}

	return false;
}

bool DrawPolylineOP::clear()
{
	if (ZoomViewOP::clear()) return true;

	m_polyline.clear();
	m_currPos.setInvalid();

	return false;
}

bool DrawPolylineOP::shouldFixMousePos() const
{
	return !m_polyline.empty() && wxGetKeyState(WXK_SHIFT);
}

void DrawPolylineOP::fixPosTo45Degree(Vector& pos) const
{
	Vector last = m_polyline.back();

	float nearest;
	Vector fixed = pos;

	const float dx = fabs(pos.x - last.x);
	nearest = dx;
	fixed.set(last.x, pos.y);

	const float dy = fabs(pos.y - last.y);
	if (dy < nearest)
	{
		nearest = dy;
		fixed.set(pos.x, last.y);
	}

	Vector other(last.x + 1, last.y - 1);
	const float dxyDown = Math::getDisPointToStraightLine(pos, last, other);
	if (dxyDown < nearest)
	{
		nearest = dxyDown;
		Math::getFootOfPerpendicular(last, other, pos, &fixed);
	}

	other.set(last.x + 1, last.y + 1);
	const float dxyUp = Math::getDisPointToStraightLine(pos, last, other);
	if (dxyUp < nearest)
	{
		nearest = dxyUp;
		Math::getFootOfPerpendicular(last, other, pos, &fixed);
	}

	pos = fixed;
}

} // d2d