#include "StagePanel.h"
#include "StageCanvas.h"

namespace etext
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(GetStageImpl(), library)
{
	SetEditOP(new d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>(this, GetStageImpl(), this));
	SetCanvas(new StageCanvas(this));
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, wxGLContext* glctx,
					   d2d::ISprite* edited, const d2d::MultiSpritesImpl* bg_sprites, 
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(GetStageImpl(), library)
{
	SetEditOP(new d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>(this, GetStageImpl(), this));
	SetCanvas(new StageCanvas(this, glctx, edited, bg_sprites));	
}

StagePanel::~StagePanel()
{	
}

}