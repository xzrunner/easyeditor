#include "ToolbarPanel.h"

namespace d2d
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, EditPanelImpl* stage, bool vertical)
	: AbstractEditCMPT(parent, wxT("Toolbar"), stage, wxT("Operation"), vertical)
{
}

} // d2d