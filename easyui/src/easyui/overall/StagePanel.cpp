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
}

void StagePanel::Clear()
{
}

bool StagePanel::InsertSprite(d2d::ISprite* sprite, int idx)
{
	bool ret = d2d::SpritesPanelImpl::InsertSprite(sprite);
	m_anchor_mgr.Insert(sprite);
	return ret;
}

bool StagePanel::RemoveSprite(d2d::ISprite* sprite)
{
	bool ret = d2d::SpritesPanelImpl::RemoveSprite(sprite);
	m_anchor_mgr.Remove(sprite);
	return ret;
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