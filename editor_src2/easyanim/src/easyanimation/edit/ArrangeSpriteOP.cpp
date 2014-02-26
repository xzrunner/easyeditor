#include "ArrangeSpriteOP.h"

#include "view/StagePanel.h"
#include "view/StageSettings.h"
#include "frame/Context.h"

namespace eanim
{

ArrangeSpriteOP::ArrangeSpriteOP(StagePanel* stage)
	: d2d::ArrangeSpriteOP<SelectSpritesOP>(stage, stage, 
	static_cast<d2d::PropertySettingPanel*>(Context::Instance()->property))
	, m_settings(stage->settings)
	, m_bMoveCenter(false)
{
}

bool ArrangeSpriteOP::onMouseLeftDown(int x, int y)
{
	if (d2d::ArrangeSpriteOP<SelectSpritesOP>::onMouseLeftDown(x, y)) 
		return true;

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	m_bMoveCenter = d2d::Math::getDistance(pos, m_center) < RADIUS;

	return false;
}

bool ArrangeSpriteOP::onMouseLeftUp(int x, int y)
{
	if (d2d::ArrangeSpriteOP<SelectSpritesOP>::onMouseLeftUp(x, y)) 
		return true;

	m_bMoveCenter = false;

	return false;
}

bool ArrangeSpriteOP::onMouseDrag(int x, int y)
{
	if (m_bMoveCenter)
	{
		m_center = m_editPanel->transPosScreenToProject(x, y);
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

	if (m_settings.bDrawCross)
	{
		const d2d::Colorf COLOR = d2d::Colorf(1, 0, 0);
		d2d::PrimitiveDraw::drawCircle(m_center, RADIUS, false, 2, COLOR);
		d2d::PrimitiveDraw::drawCross(m_center, LENGTH, COLOR);
	}

	return false;
}

} // eanim