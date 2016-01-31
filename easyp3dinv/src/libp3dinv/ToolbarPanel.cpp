#include "ToolBarPanel.h"
#include "StagePanel.h"

namespace ep3dinv
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage)
	: ee::ToolbarPanel(parent, stage->GetStageImpl())
{
}

wxSizer* ToolbarPanel::InitLayout()
{
	return NULL;
}

}