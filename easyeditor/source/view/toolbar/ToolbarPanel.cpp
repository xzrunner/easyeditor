#include "ToolbarPanel.h"

namespace ee
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, EditPanelImpl* stage, bool vertical)
	: EditCMPT(parent, "Toolbar", stage, "Operation", vertical)
{
}

}