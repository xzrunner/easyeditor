#include "StagePanel.h"
#include "StageCanvas.h"
#include "ToolBarPanel.h"
#include "EditOP.h"
#include "TopPannels.h"
#include "TopLibraryPanel.h"
#include "TopToolbarPanel.h"

namespace eui
{
namespace list
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   wxGLContext* glctx, TopPannels* top_pannels)
	: UIStagePage(parent, frame)
	, ee::MultiSpritesImpl(GetStageImpl())
	, m_top_pannels(top_pannels)
{
	SetEditOP(static_cast<EditClipboxOP*>(new EditOP(this, top_pannels->property)));
	SetCanvas(new StageCanvas(this, glctx));

	ee::LibraryPanel* library = top_pannels->library->GetRawLibrary();
	SetDropTarget(new ee::SpriteDropTarget(GetStageImpl(), library));

	top_pannels->property->SetEditPanel(GetStageImpl());

	m_toolbar = new ToolbarPanel(top_pannels->toolbar, this);
	m_toolbar_idx = top_pannels->toolbar->AddToolbar(m_toolbar);

	RegistSubject(ee::InsertSpriteSJ::Instance());
	RegistSubject(ee::ClearSpriteSJ::Instance());
}

void StagePanel::TraverseSprites(ee::Visitor& visitor, ee::DataTraverseType type, bool order) const
{
	m_list.TraverseSprites(visitor);
}

void StagePanel::LoadFromFile(const char* filename)
{
	m_list.LoadFromFile(filename);

	m_toolbar->EnableHori(m_list.IsHoriEnable());
	m_toolbar->EnableVert(m_list.IsVertEnable());

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void StagePanel::StoreToFile(const char* filename) const
{
	m_list.StoreToFile(filename);
}

void StagePanel::EnablePage(bool enable)
{
	if (enable) {
		m_top_pannels->toolbar->EnableToolbar(m_toolbar_idx);
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
		m_top_pannels->library->EnableUILibrary(false);
	} else {
		GetSpriteSelection()->Clear();
	}

	EnableObserve(enable);
	GetStageImpl()->EnableObserve(enable);
}

void StagePanel::OnNotify(int sj_id, void* ud)
{
	MultiSpritesImpl::OnNotify(sj_id, ud);

	switch (sj_id)
	{
	case ee::MSG_INSERT_SPRITE:
		{
			ee::InsertSpriteSJ::Params* p = (ee::InsertSpriteSJ::Params*)ud;
			if (m_list.InsertSprite(p->spr, p->idx)) {
				ee::SetCanvasDirtySJ::Instance()->SetDirty();
			}		
		}
		break;
	case ee::MSG_CLEAR_SPRITE:
		if (m_list.ClearAllSprite()) {
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
		}
		break;
	}
}

}
}