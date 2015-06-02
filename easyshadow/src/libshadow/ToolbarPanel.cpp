#include "ToolBarPanel.h"
#include "StagePanel.h"

namespace eshadow
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage)
	: d2d::ToolbarPanel(parent, stage)
{
}

wxSizer* ToolbarPanel::initLayout()
{
	return NULL;
}

}