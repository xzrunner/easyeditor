#include "ViewlistList.h"
#include "ViewlistPanel.h"

namespace d2d
{

ViewlistList::ViewlistList(ViewlistPanel* panel)
	: VerticalImageList(panel, "viewlist", false)
	, m_panel(panel)
{
}

void ViewlistList::onListSelected(wxCommandEvent& event)
{
	m_panel->onSelected(event.GetInt());
}

void ViewlistList::onListDoubleClicked(wxCommandEvent& event)
{
	m_panel->onSelected(event.GetInt());
}

} // d2d