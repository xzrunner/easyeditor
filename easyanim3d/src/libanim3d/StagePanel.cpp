#include "StagePanel.h"
#include "StageCanvas.h"

#include "easy3d/ControlCameraOP.h"

namespace eanim3d
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   ee::LibraryPanel* library)
	: ee::EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), library)
{
	SetCanvas(new StageCanvas(this, GetStageImpl(), this));
	SetEditOP(new e3d::ControlCameraOP(this, GetStageImpl()));
}

void StagePanel::OnSizeDebug(wxSizeEvent& event)
{
//	m_canvas->SetSize(event.GetSize()); 
}

}