#include "ArrangeSpriteOP.h"

#include "view/StagePanel.h"
#include "frame/Context.h"

namespace eanim
{

ArrangeSpriteOP::ArrangeSpriteOP(StagePanel* stage, 
								 d2d::PropertySettingPanel* property)
	: d2d::ArrangeSpriteOP<SelectSpritesOP>(stage, stage, property)
	, m_keyDownHandler(this)
	, m_selected(NULL)
{
}

ArrangeSpriteOP::~ArrangeSpriteOP()
{
	for_each(m_crosses.begin(), m_crosses.end(), DeletePointerFunctor<Cross>());
}

bool ArrangeSpriteOP::onKeyDown(int keyCode)
{
	if (d2d::ArrangeSpriteOP<SelectSpritesOP>::onKeyDown(keyCode)) 
		return true;

	m_keyDownHandler.process(keyCode);

	return false;
}

bool ArrangeSpriteOP::onMouseLeftDown(int x, int y)
{
	if (d2d::ArrangeSpriteOP<SelectSpritesOP>::onMouseLeftDown(x, y)) 
		return true;

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	for (int i = 0, n = m_crosses.size(); i < n; ++i) {
		if (m_crosses[i]->contain(pos)) {
			m_selected = m_crosses[i];
		}
	}	

	return false;
}

bool ArrangeSpriteOP::onMouseLeftUp(int x, int y)
{
	if (d2d::ArrangeSpriteOP<SelectSpritesOP>::onMouseLeftUp(x, y)) 
		return true;

	m_selected = NULL;

	return false;
}

bool ArrangeSpriteOP::onMouseDrag(int x, int y)
{
	if (m_selected)
	{
		m_selected->pos = m_editPanel->transPosScreenToProject(x, y);
		m_editPanel->Refresh();
	}
	else
	{
		if (d2d::ArrangeSpriteOP<SelectSpritesOP>::onMouseDrag(x, y)) 
			return true;
	}

	return false;
}

bool ArrangeSpriteOP::onDraw() const
{
	if (d2d::ArrangeSpriteOP<SelectSpritesOP>::onDraw()) 
		return true;

	for (int i = 0, n = m_crosses.size(); i < n; ++i) {
		m_crosses[i]->draw();
	}

	return false;
}

void ArrangeSpriteOP::addCross()
{
	m_crosses.push_back(new Cross);
	m_editPanel->Refresh();
}

void ArrangeSpriteOP::delCross()
{
	if (m_crosses.empty())
		return;

	Cross* del = m_crosses.back();
	m_crosses.pop_back();
	if (m_selected == del)
	{
		if (m_crosses.empty()) {
			m_selected = NULL;
		} else {
			m_selected = m_crosses.back();
		}
	}
	delete del;

	m_editPanel->Refresh();
}

//////////////////////////////////////////////////////////////////////////
// class ArrangeSpriteOP::Cross
//////////////////////////////////////////////////////////////////////////

ArrangeSpriteOP::Cross::Cross()
	: pos(0, 0)
{
}

void ArrangeSpriteOP::Cross::draw() const
{
	d2d::PrimitiveDraw::drawCircle(pos, RADIUS, false, 1, d2d::LIGHT_RED);
	d2d::PrimitiveDraw::cross(pos, LENGTH, d2d::LIGHT_RED, 1);
}

bool ArrangeSpriteOP::Cross::contain(const d2d::Vector& p) const
{
	return d2d::Math::getDistance(pos, p) < RADIUS;
}

} // eanim