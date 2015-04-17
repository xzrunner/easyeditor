#include "StagePanel.h"
#include "StageCanvas.h"

#include <easycomplex.h>

namespace eui
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::PropertySettingPanel* property,
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(this, library)
	, m_symbols_cfg(this, library)
	, m_viewlist(NULL)
{
	m_editOP = new d2d::ArrangeSpriteOP<ecomplex::SelectSpritesOP>(this, this, property);
	m_canvas = new StageCanvas(this);
}

void StagePanel::clear()
{
}

void StagePanel::removeSprite(d2d::ISprite* sprite)
{
	d2d::SpritesPanelImpl::removeSprite(sprite);
	m_viewlist->remove(sprite);
}

void StagePanel::insertSprite(d2d::ISprite* sprite)
{
	d2d::SpritesPanelImpl::insertSprite(sprite);
	m_viewlist->insert(sprite);
}

void StagePanel::resetSpriteOrder(d2d::ISprite* sprite, bool up)
{
	d2d::SpritesPanelImpl::resetSpriteOrder(sprite, up);
	m_viewlist->reorder(sprite, up);
}

}