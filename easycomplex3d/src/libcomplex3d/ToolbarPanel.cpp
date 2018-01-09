#include "ToolbarPanel.h"

#include <easynode3d/StagePanel.h>

namespace ecomplex3d
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, enode3d::StagePanel* stage)
	: ee::ToolbarPanel(parent, stage->GetStageImpl())
{
	SetSizer(InitLayout());	
}

wxSizer* ToolbarPanel::InitLayout()
{
	return InitChildrenLayout();
}
	
}