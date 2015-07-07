#include "StagePanel.h"
#include "StageCanvas.h"
#include "EditOP.h"

namespace ep3dinv
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(this, NULL)
	, m_ps(NULL)
{
	m_edit_op = new EditOP(this);

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