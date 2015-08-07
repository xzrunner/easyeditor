#include "ToolBarPanel.h"
#include "StagePanel.h"

namespace eanim3d
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage)
	: d2d::ToolbarPanel(parent, stage->GetStageImpl())
{
}

wxSizer* ToolbarPanel::initLayout()
{
	return NULL;
}

}