#include "ToolbarPanel.h"
#include "StagePanel.h"

namespace libsketch
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage)
	: d2d::ToolbarPanel(parent, stage->GetStageImpl())
{
	SetSizer(initLayout());	
}

wxSizer* ToolbarPanel::initLayout()
{
	return initChildrenLayout();
}
	
}