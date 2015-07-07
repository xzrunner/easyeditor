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
					   LibraryPanel* library, d2d::ViewPanelMgr* view_panel_mgr)
	: EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(parent, new SymbolContainer(m_symbol = new Symbol))
	, m_library(library)
	, m_view_panel_mgr(view_panel_mgr)
{
	m_edit_op = new d2d::ArrangeSpriteOP<SelectSpritesOP>(this, this, property, view_panel_mgr);
	m_canvas = new StageCanvas(this);

	SetDropTarget(new d2d::StageDropTarget(this, this, library));

	MODULE_STAGE.impl = this;
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   Symbol* symbol,
					   d2d::PropertySettingPanel* property,
					   LibraryPanel* library,
					   d2d::ViewPanelMgr* view_panel_mgr)
	: EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(parent, new SymbolContainer(m_symbol = symbol))
	, m_library(library)
	, m_view_panel_mgr(view_panel_mgr)
{
	m_edit_op = new d2d::ArrangeSpriteOP<SelectSpritesOP>(this, this, property, view_panel_mgr);
	m_canvas = new StageCanvas(this);

	SetDropTarget(new d2d::StageDropTarget(this, this, library));
}

void StagePanel::Clear()
{
	d2d::EditPanel::Clear();

	Symbol* symbol = getSymbol();
	for (size_t i = 0, n = symbol->m_sprites.size(); i < n; ++i)
		symbol->m_sprites[i]->Release();
	symbol->m_sprites.clear();

	symbol->m_clipbox = d2d::Rect(0, 0);
}

void StagePanel::removeSprite(d2d::ISprite* sprite)
{
	std::ofstream fout("del_debug.txt", std::ios::app);
	fout << "StagePanel::removeSprite begin" << std::endl;

	d2d::SpritesPanelImpl::removeSprite(sprite);
	if (m_view_panel_mgr) {
		m_view_panel_mgr->RemoveSprite(sprite, this);
	}

	fout << "StagePanel::removeSprite success\n\n\n" << std::endl;

	fout.close();

//	wxRemoveFile("del_debug.txt");
}

void StagePanel::insertSprite(d2d::ISprite* sprite)
{
	d2d::SpritesPanelImpl::insertSprite(sprite);
	if (m_view_panel_mgr) {
		m_view_panel_mgr->InsertSprite(sprite, this);
	}
}

bool StagePanel::resetSpriteOrder(d2d::ISprite* sprite, bool up)
{
	bool ret = d2d::SpritesPanelImpl::resetSpriteOrder(sprite, up);

	if (m_view_panel_mgr) {
		m_view_panel_mgr->ReorderSprite(sprite, up, this);
	}

	return ret;
}

} // complex