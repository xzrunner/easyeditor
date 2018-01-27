#include "ToolBarPanel.h"

#include <ee3/StagePanel.h>

namespace eanim3d
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, ee3::StagePanel* stage)
	: ee::ToolbarPanel(parent, stage->GetStageImpl())
{
}

wxSizer* ToolbarPanel::InitLayout()
{
	return NULL;
}

}