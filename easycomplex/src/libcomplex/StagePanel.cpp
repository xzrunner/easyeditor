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

#include <sprite2/Sprite.h>
#include <sprite2/UpdateParams.h>
#include <sprite2/Particle3d.h>

namespace ee { extern StageModule MODULE_STAGE; }

namespace ecomplex
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   ee::PropertySettingPanel* property,
					   LibraryPanel* library)
	: EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl())
	, m_sym()
	, m_library(library)
{
	m_sym = std::make_shared<Symbol>();
	SetContainer(std::make_shared<SymbolContainer>(m_sym));

	SetEditOP(std::make_shared<ee::ArrangeSpriteOP<SelectSpritesOP>>(
		this, GetStageImpl(), this, property, nullptr, ee::ArrangeSpriteConfig(), new ArrangeSpriteImpl(this, property)));

	SetCanvas(std::make_shared<StageCanvas>(this, library));

	SetDropTarget(new ee::StageDropTarget(this, GetStageImpl(), library));

	ee::MODULE_STAGE.impl = this;

	RegistSubject(ee::ClearSpriteSJ::Instance());
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   const std::shared_ptr<Symbol>& sym,
					   ee::PropertySettingPanel* property,
					   LibraryPanel* library,
					   wxGLContext* glctx,
					   ee::CrossGuides* guides)
	: EditPanel(parent, frame)
	, m_sym(sym)
	, ee::SpritesPanelImpl(GetStageImpl())
	, m_library(library)
{
	SetContainer(std::make_shared<SymbolContainer>(m_sym));

	auto editop = std::make_shared<ee::ArrangeSpriteOP<SelectSpritesOP>>(
		this, GetStageImpl(), this, property, nullptr, ee::ArrangeSpriteConfig(), new ArrangeSpriteImpl(this, property));
	std::dynamic_pointer_cast<SelectSpritesOP>(editop)->SetGuides(guides);
	SetEditOP(editop);

	SetCanvas(std::make_shared<StageCanvas>(this, library, glctx));

	SetDropTarget(new ee::StageDropTarget(this, GetStageImpl(), library));

	RegistSubject(ee::ClearSpriteSJ::Instance());
}

bool StagePanel::UpdateStage()
{
	bool dirty = false;
	auto& children = m_sym->GetAllChildren();
	s2::UpdateParams up;
	up.SetForce(true);
	for (int i = 0, n = children.size(); i < n; ++i) 
	{
		auto& child = children[i];
		up.SetActor(child->QueryActor(NULL));
		if (child->Update(up)) {
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