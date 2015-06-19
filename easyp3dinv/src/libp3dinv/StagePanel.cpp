#include "StagePanel.h"
#include "StageCanvas.h"

namespace ep3dinv
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::LibraryPanel* library,
					   d2d::PropertySettingPanel* property)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(this, library)
{
	m_editOP = new d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>(this, this, property);

	m_canvas = new StageCanvas(this);
}

StagePanel::~StagePanel()
{
	
}

void StagePanel::clear()
{

}

void StagePanel::OnSizeDebug(wxSizeEvent& event)
{
	m_canvas->SetSize(event.GetSize()); 
}

}