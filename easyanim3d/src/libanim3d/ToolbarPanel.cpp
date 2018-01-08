#include "ToolBarPanel.h"

#include <easy3d/StagePanel.h>

namespace eanim3d
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, e3d::StagePanel* stage)
	: ee::ToolbarPanel(parent, stage->GetStageImpl())
{
}

wxSizer* ToolbarPanel::InitLayout()
{
	return NULL;
}

}