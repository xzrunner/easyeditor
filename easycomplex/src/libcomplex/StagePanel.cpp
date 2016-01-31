#include "StagePanel.h"
#include "StageCanvas.h"
#include "SelectSpritesOP.h"
#include "ArrangeSpriteImpl.h"
#include "SymbolContainer.h"
#include "LibraryPanel.h"
#include "Symbol.h"

extern ee::StageModule MODULE_STAGE;

namespace ecomplex
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   ee::PropertySettingPanel* property,
					   LibraryPanel* library)
	: EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), new SymbolContainer(m_symbol = new Symbol))
	, m_library(library)
{
	ee::EditOP* editop = new ee::ArrangeSpriteOP<SelectSpritesOP>(this, GetStageImpl(), this, property, 
		NULL, ee::ArrangeSpriteConfig(), new ArrangeSpriteImpl(this, property));
	SetEditOP(editop);
	editop->Release();

	ee::StageCanvas* canvas = new StageCanvas(this, library);
	SetCanvas(canvas);
	canvas->Release();

	SetDropTarget(new ee::StageDropTarget(this, GetStageImpl(), library));

	MODULE_STAGE.impl = this;

	RegistSubject(ee::ClearSpriteSJ::Instance());
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   Symbol* symbol,
					   ee::PropertySettingPanel* property,
					   LibraryPanel* library,
					   wxGLContext* glctx)
	: EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), new SymbolContainer(m_symbol = symbol))
	, m_library(library)
{
	ee::EditOP* editop = new ee::ArrangeSpriteOP<SelectSpritesOP>(this, GetStageImpl(), this, property, 
		NULL, ee::ArrangeSpriteConfig(), new ArrangeSpriteImpl(this, property));
	SetEditOP(editop);
	editop->Release();

	ee::StageCanvas* canvas = new StageCanvas(this, library, glctx);
	SetCanvas(canvas);
	canvas->Release();

	SetDropTarget(new ee::StageDropTarget(this, GetStageImpl(), library));

	RegistSubject(ee::ClearSpriteSJ::Instance());
}

bool StagePanel::Update(int version)
{
	bool dirty = ee::SceneNodeMgr::Instance()->Update(1 / 30.0f);
	for (int i = 0, n = m_symbol->m_sprites.size(); i < n; ++i) {
		ee::Sprite* spr = m_symbol->m_sprites[i];
		if (spr->Update(version)) {
			dirty = true;
		}
	}
	return dirty;
}

void StagePanel::OnNotify(int sj_id, void* ud)
{
	ee::SpritesPanelImpl::OnNotify(sj_id, ud);

	if (sj_id == ee::MSG_CLEAR_SPRITE) {
		Clear();
	}
}

void StagePanel::Clear()
{
	Symbol* symbol = getSymbol();
	for (size_t i = 0, n = symbol->m_sprites.size(); i < n; ++i)
		symbol->m_sprites[i]->Release();
	symbol->m_sprites.clear();

	symbol->m_clipbox = ee::Rect(0, 0);
}

} // complex