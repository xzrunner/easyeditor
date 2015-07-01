#pragma once

#include <drag2d.h>

namespace escale9
{
	class StagePanel;
	class ToolbarPanel;

	class ComposeOP : public d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>
	{
	public:
		ComposeOP(ToolbarPanel* toolbar, StagePanel* stage, d2d::PropertySettingPanel* property, 
			d2d::ViewPanelMgr* view_panel_mgr);

		virtual bool OnActive();

	private:
		ToolbarPanel* m_toolbar;

	}; // ComposeOP
}

