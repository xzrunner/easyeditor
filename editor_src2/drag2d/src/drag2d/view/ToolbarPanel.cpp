#include "ToolbarPanel.h"

namespace d2d
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, EditPanel* editPanel)
	: AbstractEditCMPT(parent, wxT("Toolbar"), editPanel, wxT("Operation"))
{
}

} // d2d