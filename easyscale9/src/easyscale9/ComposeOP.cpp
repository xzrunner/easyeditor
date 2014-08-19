#include "ComposeOP.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "ComposeImpl.h"

namespace escale9
{

ComposeOP::ComposeOP(ToolbarPanel* toolbar, StagePanel* stage, 
					 d2d::PropertySettingPanel* property)
	: d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>(stage, stage, property, NULL, 
	false, false, new ComposeImpl(stage, property))
	, m_toolbar(toolbar)
{
}

bool ComposeOP::onActive()
{
	if (d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>::onActive()) return true;

	m_toolbar->setComposeOP(true);

	return false;
}

} // escale9