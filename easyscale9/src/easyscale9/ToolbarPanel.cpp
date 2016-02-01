#include "ToolbarPanel.h"
#include "StagePanel.h"
#include "ComposeOP.h"
#include "ResizeCMPT.h"

#include <ee/UniversalCMPT.h>

namespace escale9
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage,
						   ee::PropertySettingPanel* property)
	: ee::ToolbarPanel(parent, stage->GetStageImpl())
{
 	AddChild(new ee::UniversalCMPT(this, "compose", stage->GetStageImpl(), 
		new ComposeOP(stage, this)));
	AddChild(m_resizeCmpt = new ResizeCMPT(this, "resize", stage, this));

	SetSizer(InitLayout());	
}

wxSizer* ToolbarPanel::InitLayout()
{
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->Add(InitChildrenLayout());
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