#include "ToolbarPanel.h"

#include <ee3/StagePanel.h>

namespace ecomplex3d
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, ee3::StagePanel* stage)
	: ee::ToolbarPanel(parent, stage->GetStageImpl())
{
	SetSizer(InitLayout());	
}

wxSizer* ToolbarPanel::InitLayout()
{
	return InitChildrenLayout();
}
	
}