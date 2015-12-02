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

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, TopPannels* top_pannels)
	: UIStagePage(parent, frame)
	, d2d::MultiSpritesImpl(GetStageImpl())
	, m_top_pannels(top_pannels)
{
	SetEditOP(static_cast<EditClipboxOP*>(new EditOP(this, top_pannels->property)));
	SetCanvas(new StageCanvas(this));

	d2d::LibraryPanel* library = top_pannels->library->GetRawLibrary();
	SetDropTarget(new d2d::SpriteDropTarget(GetStageImpl(), library));
	library->SetCanvas(GetCanvas());

	top_pannels->property->SetEditPanel(GetStageImpl());

	m_toolbar = new ToolbarPanel(top_pannels->toolbar, this);
	m_toolbar_idx = top_pannels->toolbar->AddToolbar(m_toolbar);

	RegistSubject(d2d::InsertSpriteSJ::Instance());
	RegistSubject(d2d::ClearSpriteSJ::Instance());
	UnRegistSubjects();
}

void StagePanel::TraverseSprites(d2d::IVisitor& visitor, d2d::DataTraverseType type, bool order) const
{
	m_list.TraverseSprites(visitor);
}

void StagePanel::LoadFromFile(const char* filename)
{
	m_list.LoadFromFile(filename);

	m_toolbar->EnableHori(m_list.IsHoriEnable());
	m_toolbar->EnableVert(m_list.IsVertEnable());

	d2d::SetCanvasDirtySJ::Instance()->SetDirty();
}

void StagePanel::StoreToFile(const char* filename) const
{
	m_list.StoreToFile(filename);
}

void StagePanel::EnablePage(bool enable)
{
	if (enable) {
		m_top_pannels->toolbar->EnableToolbar(m_toolbar_idx);
		d2d::SetCanvasDirtySJ::Instance()->SetDirty();
		m_top_pannels->library->EnableUILibrary(false);
		RegistSubjects();
	} else {
		GetSpriteSelection()->Clear();
		UnRegistSubjects();
	}
}

void StagePanel::OnNotify(int sj_id, void* ud)
{
	MultiSpritesImpl::OnNotify(sj_id, ud);

	switch (sj_id)
	{
	case d2d::MSG_INSERT_SPRITE:
		{
			d2d::InsertSpriteSJ::Params* p = (d2d::InsertSpriteSJ::Params*)ud;
			if (m_list.InsertSprite(p->spr, p->idx)) {
				d2d::SetCanvasDirtySJ::Instance()->SetDirty();
			}		
		}
		break;
	case d2d::MSG_CLEAR_SPRITE:
		if (m_list.ClearAllSprite()) {
			d2d::SetCanvasDirtySJ::Instance()->SetDirty();
		}
		break;
	}
}

}
}