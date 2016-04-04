#include "ToolbarPanel.h"
#include "StagePanel.h"

namespace enav
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