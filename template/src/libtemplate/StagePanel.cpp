#include "StagePanel.h"
#include "StageCanvas.h"

#include <ee/ArrangeSpriteOP.h>
#include <ee/SelectSpritesOP.h>

namespace etemplate
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   ee::LibraryPanel* library,
					   ee::PropertySettingPanel* property)
	: ee::EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), library)
{
	ee::EditOP* op = new ee::ArrangeSpriteOP<ee::SelectSpritesOP>(this, GetStageImpl(), this, property);
	SetEditOP(op);
	op->Release();

	ee::StageCanvas* canvas = new StageCanvas(this);
	SetCanvas(canvas);
	canvas->Release();
}

StagePanel::~StagePanel()
{	
}

}