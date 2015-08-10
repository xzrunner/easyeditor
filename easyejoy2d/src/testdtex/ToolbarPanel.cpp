#include "ToolBarPanel.h"
#include "StagePanel.h"

namespace tdtex
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