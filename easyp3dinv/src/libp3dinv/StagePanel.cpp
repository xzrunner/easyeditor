#include "StagePanel.h"
#include "StageCanvas.h"
#include "EditOP.h"

namespace ep3dinv
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(GetStageImpl(), library)
	, m_ps(NULL)
{
	SetEditOP(new EditOP(this));
	SetCanvas(new StageCanvas(this));
}

StagePanel::~StagePanel()
{	
}

void StagePanel::OnSizeDebug(wxSizeEvent& event)
{
	GetCanvas()->SetSize(event.GetSize()); 
}

}