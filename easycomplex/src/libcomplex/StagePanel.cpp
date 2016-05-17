#include "StagePanel.h"
#include "StageCanvas.h"
#include "SelectSpritesOP.h"
#include "ArrangeSpriteImpl.h"
#include "SymbolContainer.h"
#include "LibraryPanel.h"
#include "Symbol.h"

#include <ee/wrap_StagePanel.h>
#include <ee/ArrangeSpriteOP.h>
#include <ee/StageDropTarget.h>
#include <ee/panel_msg.h>
#include <ee/SceneNode.h>
#include <ee/sprite_msg.h>
#include <ee/subject_id.h>

#include <sprite2/Sprite.h>

namespace ee { extern StageModule MODULE_STAGE; }

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

	ee::MODULE_STAGE.impl = this;

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

bool StagePanel::Update(float dt)
{
	bool dirty = ee::SceneNodeMgr::Instance()->Update(1 / 30.0f);
	const std::vector<s2::Sprite*>& children = m_symbol->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		ee::Sprite* child = static_cast<ee::Sprite*>(children[i]->GetUD());
		if (child->Update(dt)) {
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
	symbol->m_clipbox = sm::rect(sm::vec2(0, 0), 0, 0);

	m_symbol->Clear();
}

} // complex