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
#include <ee/sprite_msg.h>
#include <ee/subject_id.h>

#include <sprite2/S2_Sprite.h>
#include <sprite2/RenderParams.h>
#include <sprite2/Particle3d.h>

namespace ee { extern StageModule MODULE_STAGE; }

namespace ecomplex
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   ee::PropertySettingPanel* property,
					   LibraryPanel* library)
	: EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), new SymbolContainer(m_sym = new Symbol))
	, m_library(library)
{
	ee::EditOP* editop = new ee::ArrangeSpriteOP<SelectSpritesOP>(this, GetStageImpl(), this, property, 
		NULL, ee::ArrangeSpriteConfig(), new ArrangeSpriteImpl(this, property));
	SetEditOP(editop);
	editop->RemoveReference();

	ee::StageCanvas* canvas = new StageCanvas(this, library);
	SetCanvas(canvas);
	canvas->RemoveReference();

	SetDropTarget(new ee::StageDropTarget(this, GetStageImpl(), library));

	ee::MODULE_STAGE.impl = this;

	RegistSubject(ee::ClearSpriteSJ::Instance());
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   Symbol* sym,
					   ee::PropertySettingPanel* property,
					   LibraryPanel* library,
					   wxGLContext* glctx,
					   ee::CrossGuides* guides)
	: EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), new SymbolContainer(m_sym = sym))
	, m_library(library)
{
	ee::ArrangeSpriteOP<SelectSpritesOP>* editop = new ee::ArrangeSpriteOP<SelectSpritesOP>(
		this, GetStageImpl(), this, property, NULL, ee::ArrangeSpriteConfig(), new ArrangeSpriteImpl(this, property));
	static_cast<SelectSpritesOP*>(editop)->SetGuides(guides);
	SetEditOP(editop);
	editop->RemoveReference();

	ee::StageCanvas* canvas = new StageCanvas(this, library, glctx);
	SetCanvas(canvas);
	canvas->RemoveReference();

	SetDropTarget(new ee::StageDropTarget(this, GetStageImpl(), library));

	RegistSubject(ee::ClearSpriteSJ::Instance());
}

bool StagePanel::Update(float dt)
{
	bool dirty = s2::Particle3d::Instance()->Update(dt);
	const std::vector<s2::Sprite*>& children = m_sym->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		if (children[i]->Update(s2::RenderParams(), dt)) {
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
	m_sym->SetScissor(sm::rect(sm::vec2(0, 0), 0, 0));
	m_sym->Clear();
}

} // complex