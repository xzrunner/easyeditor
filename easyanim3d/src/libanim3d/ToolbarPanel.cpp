#include "ToolBarPanel.h"

#include <easynode3d/StagePanel.h>

namespace eanim3d
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, enode3d::StagePanel* stage)
	: ee::ToolbarPanel(parent, stage->GetStageImpl())
{
}

wxSizer* ToolbarPanel::InitLayout()
{
	return NULL;
}

}