#include "StagePanel.h"
#include "StageCanvas.h"
#include "ArrangeSpriteImpl.h"

#include "dataset/TopPannels.h"
#include "view/LibraryPanel.h"

#include <easycomplex.h>

namespace eui
{
namespace overall
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, TopPannels* top_pannels)
	: IUIStagePage(parent, frame)
	, d2d::SpritesPanelImpl(GetStageImpl(), top_pannels->library->GetUILibrary())
	, m_top_pannels(top_pannels)
	, m_symbols_cfg(this, top_pannels->library->GetUILibrary())
{
	SetEditOP(new d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>(this, GetStageImpl(), this, top_pannels->property, 
		NULL, d2d::ArrangeSpriteConfig(), new ArrangeSpriteImpl(this, top_pannels->property)));
	SetCanvas(new StageCanvas(this));

	d2d::InsertSpriteSJ::Instance()->Register(this);
	d2d::RemoveSpriteSJ::Instance()->Register(this);
}

StagePanel::~StagePanel()
{
	d2d::InsertSpriteSJ::Instance()->UnRegister(this);
	d2d::RemoveSpriteSJ::Instance()->UnRegister(this);	
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
	}
}

void StagePanel::OnSelected()
{
	SetCanvasDirty();

	m_top_pannels->library->EnableUILibrary(true);
}

void StagePanel::LoadFromFile(const char* filename)
{
	SetCanvasDirty();
}

void StagePanel::StoreToFile(const char* filename) const
{
}

void StagePanel::InitConfig()
{
	m_symbols_cfg.LoadConfig();
}

}
}