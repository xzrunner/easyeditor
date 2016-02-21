#include "StagePanel.h"
#include "StageCanvas.h"

#include <ee/ArrangeSpriteOP.h>
#include <ee/SelectSpritesOP.h>

namespace eanim3d
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   ee::LibraryPanel* library,
					   ee::PropertySettingPanel* property)
	: ee::EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), library)
{
	SetEditOP(new ee::ArrangeSpriteOP<ee::SelectSpritesOP>(this, GetStageImpl(), this, property));
	SetCanvas(new StageCanvas(this, GetStageImpl(), this));
}

StagePanel::~StagePanel()
{	
}

void StagePanel::OnSizeDebug(wxSizeEvent& event)
{
	m_canvas->SetSize(event.GetSize()); 
}

}