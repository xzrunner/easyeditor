#include "StagePanel.h"

#include "StageCanvas.h"
#include "Context.h"
#include "SelectSpritesOP.h"
#include "TreeCtrl.h"
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

#include <sprite2/S2_Sprite.h>
#include <sprite2/SymType.h>
#include <gum/StringHelper.h>

namespace edb
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   wxTreeCtrl* tree)
	: ee::EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), (ee::LibraryPanel*)(NULL))
	, ee::ShapesPanelImpl()
{
	m_db = new Database;
	m_sym = new Symbol(m_db);

	ee::EditOP* editop = new ee::ArrangeSpriteOP<SelectSpritesOP>(this, this->GetStageImpl(), this, 
		NULL, NULL, ee::ArrangeSpriteConfig(false, false, false, false));
	SetEditOP(editop);
	editop->RemoveReference();

	ee::StageCanvas* canvas = new StageCanvas(this);
	SetCanvas(canvas);
	canvas->RemoveReference();

	SetDropTarget(new DropTarget(this, this, tree));
}

StagePanel::~StagePanel()
{
	delete m_sym;
	delete m_db;
}

void StagePanel::LoadFromDir(const std::string& dirpath)
{
	m_db->Build(dirpath);
	Context::Instance()->tree->Build(*m_db);
}

/************************************************************************/
/* class StagePanel::DropTarget                                         */
/************************************************************************/

StagePanel::DropTarget::DropTarget(wxWindow* stage_wnd, StagePanel* stage, wxTreeCtrl* tree)
	: CombinedDropTarget(stage_wnd)
	, m_stage(stage)
	, m_tree(tree)
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