#include "StagePanel.h"
#include "StageCanvas.h"
#include "ArrangeSpriteImpl.h"

#include <easycomplex.h>

namespace eui
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::PropertySettingPanel* property,
					   d2d::LibraryPanel* library,
					   d2d::ViewPanelMgr* view_panel_mgr)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(GetStageImpl(), library)
	, m_symbols_cfg(this, library)
{
	SetEditOP(new d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>(this, GetStageImpl(), this, property, 
		view_panel_mgr, NULL, d2d::ArrangeSpriteConfig(), new ArrangeSpriteImpl(this, property)));
	SetCanvas(new StageCanvas(this));
}

void StagePanel::Clear()
{
}

bool StagePanel::InsertSprite(d2d::ISprite* sprite, int idx)
{
	bool ret = d2d::SpritesPanelImpl::InsertSprite(sprite);
	m_anchor_mgr.Insert(sprite);
	return ret;
}

bool StagePanel::RemoveSprite(d2d::ISprite* sprite)
{
	bool ret = d2d::SpritesPanelImpl::RemoveSprite(sprite);
	m_anchor_mgr.Remove(sprite);
	return ret;
}

void StagePanel::InitConfig()
{
	m_symbols_cfg.LoadConfig();
}

}