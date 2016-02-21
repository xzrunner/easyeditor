#include "ToolbarPanel.h"
#include "StagePanel.h"

namespace esketch
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage)
	: ee::ToolbarPanel(parent, stage->GetStageImpl())
{
	SetSizer(InitLayout());	
}

wxSizer* ToolbarPanel::InitLayout()
{
	return InitChildrenLayout();
}
	
}