#include "StagePanel.h"
#include "StageCanvas.h"
#include "SelectSpritesOP.h"
#include "ArrangeSpriteImpl.h"
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
	, d2d::SpritesPanelImpl(GetStageImpl(), new SymbolContainer(m_symbol = new Symbol))
	, m_library(library)
{
	d2d::AbstractEditOP* editop = new d2d::ArrangeSpriteOP<SelectSpritesOP>(this, GetStageImpl(), this, property, 
		NULL, d2d::ArrangeSpriteConfig(), new ArrangeSpriteImpl(this, property));
	SetEditOP(editop);
	editop->Release();

	d2d::IStageCanvas* canvas = new StageCanvas(this, library);
	SetCanvas(canvas);
	canvas->Release();

	SetDropTarget(new d2d::StageDropTarget(this, GetStageImpl(), library));

	MODULE_STAGE.impl = this;

	RegistSubject(d2d::ClearSpriteSJ::Instance());
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   Symbol* symbol,
					   d2d::PropertySettingPanel* property,
					   LibraryPanel* library,
					   wxGLContext* glctx)
	: EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(GetStageImpl(), new SymbolContainer(m_symbol = symbol))
	, m_library(library)
{
	d2d::AbstractEditOP* editop = new d2d::ArrangeSpriteOP<SelectSpritesOP>(this, GetStageImpl(), this, property, 
		NULL, d2d::ArrangeSpriteConfig(), new ArrangeSpriteImpl(this, property));
	SetEditOP(editop);
	editop->Release();

	d2d::IStageCanvas* canvas = new StageCanvas(this, library, glctx);
	SetCanvas(canvas);
	canvas->Release();

	SetDropTarget(new d2d::StageDropTarget(this, GetStageImpl(), library));

	RegistSubject(d2d::ClearSpriteSJ::Instance());
}

bool StagePanel::Update(int version)
{
	bool dirty = d2d::SceneNodeMgr::Instance()->Update(1 / 30.0f);
	for (int i = 0, n = m_symbol->m_sprites.size(); i < n; ++i) {
		d2d::Sprite* spr = m_symbol->m_sprites[i];
		if (spr->Update(version)) {
			dirty = true;
		}
	}
	return dirty;
}

void StagePanel::OnNotify(int sj_id, void* ud)
{
	d2d::SpritesPanelImpl::OnNotify(sj_id, ud);

	if (sj_id == d2d::MSG_CLEAR_SPRITE) {
		Clear();
	}
}

void StagePanel::Clear()
{
	Symbol* symbol = getSymbol();
	for (size_t i = 0, n = symbol->m_sprites.size(); i < n; ++i)
		symbol->m_sprites[i]->Release();
	symbol->m_sprites.clear();

	symbol->m_clipbox = d2d::Rect(0, 0);
}

} // complex