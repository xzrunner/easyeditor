#include "ToolbarPanel.h"
#include "StagePanel.h"

namespace esketch
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, d2d::LibraryPanel* library, 
						   d2d::PropertySettingPanel* property, StagePanel* stage)
	: d2d::ToolbarPanel(parent, stage)
{
	SetSizer(initLayout());	
}

wxSizer* ToolbarPanel::initLayout()
{
	return initChildrenLayout();
}
	
}