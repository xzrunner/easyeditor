#include "StagePanel.h"
#include "StageCanvas.h"

namespace etemplate
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::LibraryPanel* library,
					   d2d::PropertySettingPanel* property)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(GetStageImpl(), library)
{
	SetEditOP(new d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>(this, GetStageImpl(), this, property));
	SetCanvas(new StageCanvas(this));
}

StagePanel::~StagePanel()
{	
}

void StagePanel::OnSizeDebug(wxSizeEvent& event)
{
	m_canvas->SetSize(event.GetSize()); 
}

}