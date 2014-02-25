#include "ToolbarPanel.h"

namespace d2d
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, EditPanel* editPanel, bool vertical)
	: AbstractEditCMPT(parent, wxT("Toolbar"), editPanel, wxT("Operation"), vertical)
{
}

} // d2d