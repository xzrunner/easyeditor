#include "ToolbarPanel.h"

namespace emesh
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, d2d::EditPanel* stage)
	: d2d::ToolbarPanel(parent, stage)
{
}

wxSizer* ToolbarPanel::initLayout()
{
	return NULL;
}

}