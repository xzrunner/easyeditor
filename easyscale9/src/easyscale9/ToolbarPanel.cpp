#include "ToolbarPanel.h"
#include "StagePanel.h"
#include "ComposeOP.h"
#include "ResizeCMPT.h"

namespace escale9
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage,
						   d2d::PropertySettingPanel* property)
	: d2d::ToolbarPanel(parent, stage->GetStageImpl())
{
 	addChild(new d2d::UniversalCMPT(this, wxT("compose"), stage->GetStageImpl(), 
		new ComposeOP(stage, this)));
	addChild(m_resizeCmpt = new ResizeCMPT(this, wxT("resize"), stage, this));

	SetSizer(initLayout());	
}

wxSizer* ToolbarPanel::initLayout()
{
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->Add(initChildrenLayout());
	return topSizer;
}

void ToolbarPanel::setSize(float width, float height)
{
	m_resizeCmpt->setSize(width, height);
}

float ToolbarPanel::getWidth() const
{
	return m_resizeCmpt->getWidth();
}

float ToolbarPanel::getHeight() const
{
	return m_resizeCmpt->getHeight();
}

} // escale9