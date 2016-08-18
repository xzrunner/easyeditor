#include "StagePanel.h"
#include "StageCanvas.h"
#include "Symbol.h"
#include "LibraryPanel.h"

#include <ee/ZoomViewOP.h>
#include <ee/StageDropTarget.h>
#include <ee/subject_id.h>
#include <ee/sprite_msg.h>
#include <ee/Visitor.h>
#include <ee/Sprite.h>

#include <sprite2/RenderParams.h>

namespace emask
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, LibraryPanel* library)
	: ee::EditPanel(parent, frame)
	, ee::MultiSpritesImpl(GetStageImpl())
	, m_library(library)
	, m_sym(new Symbol)
{
	ee::EditOP* op = new ee::ZoomViewOP(this, GetStageImpl(), true);
	SetEditOP(op);
	op->RemoveReference();

	ee::StageCanvas* canvas = new StageCanvas(this);
	SetCanvas(canvas);
	canvas->RemoveReference();

	SetDropTarget(new ee::StageDropTarget(this, GetStageImpl(), library));
}

StagePanel::~StagePanel()
{	
	m_sym->RemoveReference();
}

bool StagePanel::Update(float dt)
{
	m_sym->Update(s2::RenderParams(), dt);
	return true;
}

void StagePanel::SetSymbol(Symbol* sym) 
{ 
	cu::RefCountObjAssign(m_sym, sym); 
}

void StagePanel::OnNotify(int sj_id, void* ud)
{
	ee::MultiSpritesImpl::OnNotify(sj_id, ud);

	switch (sj_id)
	{
	case ee::MSG_INSERT_SPRITE:
		{
			ee::InsertSpriteSJ::Params* p = (ee::InsertSpriteSJ::Params*)ud;
			m_sym->SetSymbol(dynamic_cast<const ee::Symbol*>(p->spr->GetSymbol()), m_library->IsCurrBase());
		}
		break;
	}
}

}