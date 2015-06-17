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
	, d2d::SpritesPanelImpl(this, library)
	, m_symbols_cfg(this, library)
{
	m_editOP = new d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>(this, this, property, 
		view_panel_mgr, NULL, d2d::ArrangeSpriteConfig(), new ArrangeSpriteImpl(this, property));
	m_canvas = new StageCanvas(this);
}

void StagePanel::clear()
{
}

void StagePanel::removeSprite(d2d::ISprite* sprite)
{
	d2d::SpritesPanelImpl::removeSprite(sprite);
	m_anchor_mgr.Remove(sprite);
}

void StagePanel::insertSprite(d2d::ISprite* sprite)
{
	d2d::SpritesPanelImpl::insertSprite(sprite);
	m_anchor_mgr.Insert(sprite);
}

void StagePanel::InitConfig()
{
	m_symbols_cfg.LoadConfig();
}

}