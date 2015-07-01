#include "ComposeOP.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "ComposeImpl.h"

namespace escale9
{

ComposeOP::ComposeOP(ToolbarPanel* toolbar, StagePanel* stage, 
					 d2d::PropertySettingPanel* property,
					 d2d::ViewPanelMgr* view_panel_mgr)
	: d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>(stage, stage, property, view_panel_mgr, NULL, 
	d2d::ArrangeSpriteConfig(), new ComposeImpl(stage, property))
	, m_toolbar(toolbar)
{
}

bool ComposeOP::OnActive()
{
	if (d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>::OnActive()) return true;

	m_toolbar->setComposeOP(true);

	return false;
}

} // escale9