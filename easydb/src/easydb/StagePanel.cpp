#include "StagePanel.h"
#include "StageCanvas.h"
#include "SelectSpritesOP.h"
#include "Database.h"
#include "Symbol.h"
#include "Sprite.h"

#include <ee/ArrangeSpriteOP.h>
#include <ee/sprite_msg.h>
#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/SpriteFactory.h>
#include <ee/FetchAllVisitor.h>
#include <ee/sprite_msg.h>
#include <ee/Random.h>
#include <ee/SymbolFile.h>
#include <ee/sprite_msg.h>

#include <easycomplex.h>
#include <easyanim.h>

#include <sprite2/Sprite.h>
#include <sprite2/SymType.h>
#include <gum/StringHelper.h>

namespace edb
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame)
	: ee::EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), (ee::LibraryPanel*)(NULL))
	, ee::ShapesPanelImpl()
{
	m_db = new Database;
	m_sym = new Symbol(m_db);

	SetEditOP(std::make_shared<ee::ArrangeSpriteOP<SelectSpritesOP>>(
		this, this->GetStageImpl(), this, NULL, NULL, ee::ArrangeSpriteConfig(false, false, false, false)));
	SetCanvas(std::make_shared<StageCanvas>(this));

	SetDropTarget(new DropTarget(this, this));
}

StagePanel::~StagePanel()
{
	delete m_sym;
	delete m_db;
}

/************************************************************************/
/* class StagePanel::DropTarget                                         */
/************************************************************************/

StagePanel::DropTarget::DropTarget(wxWindow* stage_wnd, StagePanel* stage)
	: CombinedDropTarget(stage_wnd)
	, m_stage(stage)
{
}

void StagePanel::DropTarget::OnDropText(wxCoord x, wxCoord y, const wxString& text)
{
	int id;
	gum::StringHelper::FromString(text.ToStdString(), id);
	Sprite* spr = new Sprite(m_stage->GetSymbol(), id);
	
	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	spr->Translate(pos);
	ee::InsertSpriteSJ::Instance()->Insert(spr);
}

}