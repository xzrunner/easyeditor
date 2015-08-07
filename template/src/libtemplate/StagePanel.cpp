#include "StagePanel.h"
#include "StageCanvas.h"

namespace etemplate
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::LibraryPanel* library,
					   d2d::PropertySettingPanel* property,
					   d2d::ViewPanelMgr* view_panel_mgr)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(GetStageImpl(), library)
{
	m_edit_op = new d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>(this, this, property, view_panel_mgr);

	m_canvas = new StageCanvas(this);
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