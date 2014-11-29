#include "AutoRectCutOP.h"
#include "StagePanel.h"

namespace eimage
{

AutoRectCutOP::AutoRectCutOP(d2d::EditPanel* editpanel)
	: d2d::ZoomViewOP(editpanel, true, false, false)
	, m_selected(NULL)
{
}

bool AutoRectCutOP::onMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftDown(x, y)) return true;

	m_last_pos = m_editPanel->transPosScreenToProject(x, y);
	m_selected = m_rects.queryRect(m_last_pos);
	m_editPanel->Refresh();

	return false;
}

bool AutoRectCutOP::onMouseLeftUp(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftUp(x, y)) return true;

	if (m_selected) {
		m_selected->xMin = (int)(m_selected->xMin+0.5f);
		m_selected->xMax = (int)(m_selected->xMax+0.5f);
		m_selected->yMin = (int)(m_selected->yMin+0.5f);
		m_selected->yMax = (int)(m_selected->yMax+0.5f);
		m_editPanel->Refresh();
	}

	return false;
}

bool AutoRectCutOP::onMouseRightDown(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseRightDown(x, y)) return true;

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	bool removed = m_rects.remove(pos);
	if (removed) {
		m_selected = NULL;
		m_editPanel->Refresh();
	}

	return false;
}

bool AutoRectCutOP::onMouseDrag(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseDrag(x, y)) return true;

	if (m_selected)
	{
		d2d::Vector curr = m_editPanel->transPosScreenToProject(x, y);
		m_rects.moveRect(m_selected, m_last_pos, curr);
		m_last_pos = curr;
		m_editPanel->Refresh();
	}

	return false;
}

bool AutoRectCutOP::onMouseLeftDClick(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftDClick(x, y)) return true;

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	d2d::Rect* r = m_rects.queryRect(pos);
	if (r) {
		m_rects.insert(d2d::Rect(*r), true);
		m_last_pos = pos;
		m_selected = r;
	}

	return false;
}

bool AutoRectCutOP::onDraw() const
{
	if (d2d::ZoomViewOP::onDraw()) return true;

	d2d::PrimitiveDraw::cross(d2d::Vector(0, 0), 100, 100, d2d::Colorf(1, 0, 0));

	m_rects.draw();

	if (m_selected) {
		d2d::PrimitiveDraw::rect(*m_selected, d2d::LIGHT_GREEN_FACE);
	}

	return false;
}

bool AutoRectCutOP::clear()
{
	if (d2d::ZoomViewOP::clear()) return true;

	m_rects.clear();

	return false;
}

}