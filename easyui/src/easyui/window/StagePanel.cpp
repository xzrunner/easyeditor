#include "StagePanel.h"
#include "StageCanvas.h"
#include "ArrangeSpriteImpl.h"
#include "TopPannels.h"
#include "TopLibraryPanel.h"
#include "ToolbarPanel.h"
#include "TopToolbarPanel.h"
#include "message_id.h"
#include "QueryWindowViewSizeSJ.h"

#include <easycomplex.h>

namespace eui
{
namespace window
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, TopPannels* top_pannels)
	: UIStagePage(parent, frame)
	, d2d::SpritesPanelImpl(GetStageImpl(), top_pannels->library->GetUILibrary())
	, m_top_pannels(top_pannels)
	, m_symbols_cfg(this, top_pannels->library->GetUILibrary())
{
	SetEditOP(new d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>(this, GetStageImpl(), this, top_pannels->property, 
		NULL, d2d::ArrangeSpriteConfig(), new ArrangeSpriteImpl(this, top_pannels->property)));
	SetCanvas(new StageCanvas(this));

	m_view_width = d2d::HALF_S_WIDTH * 2;
	m_view_height = d2d::HALF_S_HEIGHT * 2;

	m_toolbar = new ToolbarPanel(top_pannels->toolbar, this);
	m_toolbar_idx = top_pannels->toolbar->AddToolbar(m_toolbar);

	AddSubject(d2d::InsertSpriteSJ::Instance());
	AddSubject(d2d::RemoveSpriteSJ::Instance());
	AddSubject(QueryWindowViewSizeSJ::Instance());
	UnRegistSubjects(this);
}

StagePanel::~StagePanel()
{
	UnRegistSubjects(this);
}

void StagePanel::Notify(int sj_id, void* ud)
{
	d2d::SpritesPanelImpl::Notify(sj_id, ud);

	switch (sj_id)
	{
	case d2d::MSG_INSERT_SPRITE:
		m_anchor_mgr.Insert(((d2d::InsertSpriteSJ::Params*)ud)->spr);
		break;
	case d2d::MSG_REMOVE_SPRITE:
		m_anchor_mgr.Remove((d2d::ISprite*)ud);
		break;
	case MSG_QUERY_WINDOW_VIEW_SIZE:
		{
			QueryWindowViewSizeSJ::Params* p = (QueryWindowViewSizeSJ::Params*)ud;
			p->width = m_view_width;
			p->height = m_view_height;
		}
		break;
	}
}

void StagePanel::LoadFromFile(const char* filename)
{
	SetCanvasDirty();
}

void StagePanel::StoreToFile(const char* filename) const
{
}

void StagePanel::EnablePage(bool enable)
{
	if (enable) {
		m_top_pannels->toolbar->EnableToolbar(m_toolbar_idx);
		SetCanvasDirty();
		m_top_pannels->library->EnableUILibrary(true);
		RegistSubjects(this);
	} else {
		GetSpriteSelection()->Clear();
		UnRegistSubjects(this);
	}
}

void StagePanel::InitConfig()
{
	m_symbols_cfg.LoadConfig();
}

void StagePanel::SetViewSize(int width, int height)
{
	if (m_view_width == width && 
		m_view_height == height) {
		return;
	}

	m_view_width = width;
	m_view_height = height;
	m_anchor_mgr.OnViewChanged(m_view_width, m_view_height);
}

}
}