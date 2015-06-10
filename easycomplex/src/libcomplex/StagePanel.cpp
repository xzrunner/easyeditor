#include "StagePanel.h"
#include "StageCanvas.h"
#include "SelectSpritesOP.h"
#include "SymbolContainer.h"
#include "LibraryPanel.h"
#include "Symbol.h"

extern d2d::StageModule MODULE_STAGE;

namespace ecomplex
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   d2d::PropertySettingPanel* property,
					   LibraryPanel* library)
	: EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(parent, new SymbolContainer(m_symbol = new Symbol))
	, m_library(library)
	, m_viewlist(NULL)
	, m_group_panel(NULL)
{
	m_editOP = new d2d::ArrangeSpriteOP<SelectSpritesOP>(this, this, property);
	m_canvas = new StageCanvas(this);

	SetDropTarget(new d2d::StageDropTarget(this, this, library));

	MODULE_STAGE.impl = this;
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   Symbol* symbol,
					   d2d::PropertySettingPanel* property,
					   LibraryPanel* library)
	: EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(parent, new SymbolContainer(m_symbol = symbol))
	, m_library(library)
	, m_viewlist(NULL)
	, m_group_panel(NULL)
{
	m_editOP = new d2d::ArrangeSpriteOP<SelectSpritesOP>(this, this, property);
	m_canvas = new StageCanvas(this);

	SetDropTarget(new d2d::StageDropTarget(this, this, library));
}

void StagePanel::clear()
{
	d2d::EditPanel::clear();

	Symbol* symbol = getSymbol();
	for (size_t i = 0, n = symbol->m_sprites.size(); i < n; ++i)
		symbol->m_sprites[i]->Release();
	symbol->m_sprites.clear();

	symbol->m_clipbox = d2d::Rect(0, 0);
}

void StagePanel::removeSprite(d2d::ISprite* sprite)
{
	d2d::SpritesPanelImpl::removeSprite(sprite);
	m_viewlist->Remove(sprite);
	if (m_group_panel) {
		m_group_panel->Remove(sprite);
	}
}

void StagePanel::insertSprite(d2d::ISprite* sprite)
{
	d2d::SpritesPanelImpl::insertSprite(sprite);
	m_viewlist->Insert(sprite);
}

bool StagePanel::resetSpriteOrder(d2d::ISprite* sprite, bool up)
{
	bool ret = d2d::SpritesPanelImpl::resetSpriteOrder(sprite, up);
	m_viewlist->Reorder(sprite, up);
	return ret;
}

} // complex