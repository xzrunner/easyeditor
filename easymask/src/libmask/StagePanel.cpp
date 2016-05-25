#include "StagePanel.h"
#include "StageCanvas.h"
#include "Symbol.h"
#include "LibraryPanel.h"

#include <ee/ArrangeSpriteOP.h>
#include <ee/SelectSpritesOP.h>
#include <ee/StageDropTarget.h>
#include <ee/subject_id.h>
#include <ee/sprite_msg.h>
#include <ee/Visitor.h>

namespace emask
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   ee::PropertySettingPanel* property,
					   LibraryPanel* library)
	: ee::EditPanel(parent, frame)
	, ee::MultiSpritesImpl(GetStageImpl())
	, m_library(library)
	, m_sym(new Symbol)
{
	ee::EditOP* op = new ee::ArrangeSpriteOP<ee::SelectSpritesOP>(this, GetStageImpl(), this, property);
	SetEditOP(op);
	op->Release();

	ee::StageCanvas* canvas = new StageCanvas(this);
	SetCanvas(canvas);
	canvas->Release();

	SetDropTarget(new ee::StageDropTarget(this, GetStageImpl(), library));
}

StagePanel::~StagePanel()
{	
	m_sym->Release();
}

bool StagePanel::Update(float dt)
{
	return false;
}

void StagePanel::TraverseSprites(ee::Visitor& visitor, ee::DataTraverseType type, bool order) const
{
	bool next = false;
	if (ee::Sprite* spr = m_sym->GetSprite(true)) {
		visitor.Visit(spr, next);
	}
	if (ee::Sprite* spr = m_sym->GetSprite(false)) {
		visitor.Visit(spr, next);
	}
}

void StagePanel::SetSymbol(Symbol* symbol) 
{ 
	ee::obj_assign(m_sym, symbol); 
}

void StagePanel::OnNotify(int sj_id, void* ud)
{
	ee::MultiSpritesImpl::OnNotify(sj_id, ud);

	switch (sj_id)
	{
	case ee::MSG_INSERT_SPRITE:
		{
			ee::InsertSpriteSJ::Params* p = (ee::InsertSpriteSJ::Params*)ud;
			m_sym->SetSprite(p->spr, m_library->IsCurrBase());
		}
		break;
	}
}

}