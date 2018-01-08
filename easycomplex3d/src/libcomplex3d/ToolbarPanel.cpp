#include "ToolbarPanel.h"

#include <easy3d/StagePanel.h>

namespace ecomplex3d
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, e3d::StagePanel* stage)
	: ee::ToolbarPanel(parent, stage->GetStageImpl())
{
	SetSizer(InitLayout());	
}

wxSizer* ToolbarPanel::InitLayout()
{
	return InitChildrenLayout();
}
	
}