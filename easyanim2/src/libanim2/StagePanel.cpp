#include "StagePanel.h"
#include "StageCanvas.h"

#include <ee/ArrangeSpriteOP.h>
#include <ee/SelectSpritesOP.h>

namespace libanim2
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   ee::LibraryPanel* library,
					   ee::PropertySettingPanel* property)
	: ee::EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), library)
{
	SetEditOP(std::make_shared<ee::ArrangeSpriteOP<ee::SelectSpritesOP>>(
		this, GetStageImpl(), this, property));
	SetCanvas(std::make_shared<StageCanvas>(this));
}

StagePanel::~StagePanel()
{	
}

}