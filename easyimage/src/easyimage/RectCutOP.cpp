#include "RectCutOP.h"
#include "StagePanel.h"

namespace eimage
{

RectCutOP::RectCutOP(RectCutCMPT* cmpt, StagePanel* stage)
	: d2d::ZoomViewOP(stage, true)
	, m_cmpt(cmpt)
	, m_stage(stage)
	, m_rectSelected(NULL)
{
	m_firstPos.setInvalid();
	m_currPos.setInvalid();
	m_captured.setInvalid();
}

bool RectCutOP::onMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftDown(x, y)) return true;

	if (!m_stage->getImage()) return false;

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	m_nodeSelected = m_rects.queryNode(pos);

	return false;
}

bool RectCutOP::onMouseLeftUp(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftUp(x, y)) return true;

	if (!m_stage->getImage()) return false;

	// move node
	if (m_nodeSelected.rect)
	{
		fixedPos(m_currPos);
		bool moved = m_rects.moveNode(m_nodeSelected, m_currPos);
		if (moved)
		{
			m_nodeSelected.pos = m_currPos;
			m_editPanel->Refresh();
		}
		m_nodeSelected.rect = NULL;
	}
	// fix rect
	if (m_rectSelected)
	{
		m_rectSelected->xMin = (int)(m_rectSelected->xMin + 0.5f);
		m_rectSelected->xMax = (int)(m_rectSelected->xMax + 0.5f);
		m_rectSelected->yMin = (int)(m_rectSelected->yMin + 0.5f);
		m_rectSelected->yMax = (int)(m_rectSelected->yMax + 0.5f);
		m_editPanel->Refresh();
	}

	return false;
}

bool RectCutOP::onMouseRightDown(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftDown(x, y)) return true;

	if (!m_stage->getImage()) return false;

	m_firstPos = m_editPanel->transPosScreenToProject(x, y);
	fixedPos(m_firstPos);

	return false;
}

bool RectCutOP::onMouseRightUp(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftUp(x, y)) return true;

	if (!m_stage->getImage()) return false;

	if (!m_firstPos.isValid()) {
		return false;
	}

	const float RADIUS = 5;
	// remove rect
	m_currPos = m_editPanel->transPosScreenToProject(x, y);
	if (d2d::Math::getDistance(m_currPos, m_firstPos) < RADIUS)
	{
		bool removed = m_rects.remove(m_currPos);
		if (remove) {
			m_nodeSelected.rect = NULL;
			m_rectSelected = NULL;

			m_firstPos.setInvalid();
			m_currPos.setInvalid();
			m_editPanel->Refresh();
		}
	}
	// insert rect
	else
	{
		fixedPos(m_currPos);
		if (m_firstPos.x != m_currPos.x && m_firstPos.y != m_currPos.y)
		{
			m_rects.insert(d2d::Rect(m_firstPos, m_currPos));

			m_firstPos.setInvalid();
			m_currPos.setInvalid();
			m_editPanel->Refresh();
		}
	}

	return false;
}

bool RectCutOP::onMouseMove(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseMove(x, y)) return true;

	if (!m_stage->getImage()) return false;

	m_currPos = m_editPanel->transPosScreenToProject(x, y);
	m_rectSelected = m_rects.queryRect(m_currPos);
	m_captured = m_rects.queryNearestAxis(m_currPos);
	m_editPanel->Refresh();

	return false;
}

bool RectCutOP::onMouseDrag(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseDrag(x, y)) return true;

	if (!m_stage->getImage()) return false;

	// create rect
	if (m_firstPos.isValid())
	{
		m_currPos = m_editPanel->transPosScreenToProject(x, y);
		m_captured = m_rects.queryNearestAxis(m_currPos);

		m_editPanel->Refresh();
	}
	// move rect's node
	else if (m_nodeSelected.rect)
	{
		m_currPos = m_editPanel->transPosScreenToProject(x, y);
		m_captured = m_rects.queryNearestAxis(m_currPos, m_nodeSelected.rect);

		d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
		m_rects.moveNode(m_nodeSelected, pos);
		m_nodeSelected.pos = pos;

		m_editPanel->Refresh();
	}
	// move rect
	else if (m_rectSelected)
	{
		d2d::Vector curr = m_editPanel->transPosScreenToProject(x, y);
		m_rects.moveRect(m_rectSelected, m_currPos, curr);
		m_currPos = curr;

		m_editPanel->Refresh();
	}

	return false;
}

bool RectCutOP::onDraw(const d2d::Screen& scr) const
{
	if (d2d::ZoomViewOP::onDraw(scr)) return true;

	d2d::PrimitiveDraw::cross(d2d::Vector(0, 0), 100, 100, d2d::Colorf(1, 0, 0));

	if (!m_stage->getImage()) return false;

	m_rects.draw(scr);

	if (m_firstPos.isValid() && m_currPos.isValid())
	{
		d2d::PrimitiveDraw::rect(scr, m_firstPos, m_currPos, d2d::LIGHT_RED_LINE);
	}

	drawCaptureLine(scr);

	if (m_rectSelected) {
		d2d::PrimitiveDraw::rect(scr, *m_rectSelected, d2d::LIGHT_GREEN_FACE);
	}
	if (m_nodeSelected.rect) {
		d2d::PrimitiveDraw::rect(scr, *m_nodeSelected.rect, d2d::LIGHT_GREEN_FACE);
	}

	return false;
}

bool RectCutOP::clear()
{
	if (d2d::ZoomViewOP::clear()) return true;

	m_firstPos.setInvalid();
	m_currPos.setInvalid();

	m_rects.clear();
	m_rectSelected = NULL;
	m_nodeSelected.rect = NULL;

	return false;
}

std::string RectCutOP::getImageFilepath() const
{
	if (const d2d::ISprite* s = m_stage->getImage()) {
		return s->getSymbol().getFilepath().ToStdString();
	} else {
		return "";
	}
}

void RectCutOP::loadImageFromFile(const std::string& filepath)
{
	m_stage->setImage(filepath);
}

void RectCutOP::drawCaptureLine(const d2d::Screen& scr) const
{
	if (!m_currPos.isValid()) return;
	if (m_captured.x == FLT_INVALID && m_captured.y == FLT_INVALID) return;

	const float EDGE = 4096;
	if (m_captured.x != FLT_INVALID)
	{
		d2d::Vector p0(m_captured.x, -EDGE);
		d2d::Vector p1(m_captured.x, EDGE);
		d2d::PrimitiveDraw::drawDashLine(scr, p0, p1, d2d::Colorf(0, 0, 0));
	}

	if (m_captured.y != FLT_INVALID)
	{
		d2d::Vector p0(-EDGE, m_captured.y);
		d2d::Vector p1(EDGE, m_captured.y);
		d2d::PrimitiveDraw::drawDashLine(scr, p0, p1, d2d::Colorf(0, 0, 0));
	}
}

void RectCutOP::fixedPos(d2d::Vector& pos) const
{
	const float RADIUS = 5;
	if (fabs(pos.x - m_captured.x) > RADIUS || 
		fabs(pos.y - m_captured.y) > RADIUS) {
		return;
	}
	
	// by capture
	if (m_captured.x != FLT_INVALID) {
		pos.x = m_captured.x;
	}
	if (m_captured.y != FLT_INVALID) {
		pos.y = m_captured.y;
	}

	// to int
	pos.x = std::ceil(pos.x);
	pos.y = std::ceil(pos.y);

	// to image
	float w = m_stage->getImage()->getSymbol().getSize().xLength();
	float h = m_stage->getImage()->getSymbol().getSize().yLength();
	if (pos.x < 0) {
		pos.x = 0;
	}
	if (pos.x > w) {
		pos.x = w;
	}
	if (pos.y < 0) {
		pos.y = 0;
	}
	if (pos.y > h) {
		pos.y = h;
	}
}

}