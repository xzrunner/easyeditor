#include "StagePanel.h"
#include "StageCanvas.h"

namespace eanim3d
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::LibraryPanel* library,
					   d2d::PropertySettingPanel* property)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(GetStageImpl(), library)
{
	m_edit_op = new d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>(this, this, property);

	m_canvas = new StageCanvas(this, this);
}

StagePanel::~StagePanel()
{
	
}

void StagePanel::Clear()
{

}

void StagePanel::OnSizeDebug(wxSizeEvent& event)
{
	m_canvas->SetSize(event.GetSize()); 
}

}