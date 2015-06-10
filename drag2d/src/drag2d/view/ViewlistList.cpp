#include "ViewlistList.h"
#include "ViewlistPanel.h"

namespace d2d
{

BEGIN_EVENT_TABLE(ViewlistList, VerticalImageList)
	EVT_KEY_DOWN(ViewlistList::OnKeyDown)
END_EVENT_TABLE()

ViewlistList::ViewlistList(ViewlistPanel* panel)
	: VerticalImageList(panel, "viewlist", false)
	, m_panel(panel)
{
}

void ViewlistList::onListSelected(wxCommandEvent& event)
{
	m_panel->OnSelected(event.GetInt());
}

void ViewlistList::onListDoubleClicked(wxCommandEvent& event)
{
	m_panel->OnSelected(event.GetInt());
}

void ViewlistList::OnKeyDown(wxKeyEvent& event)
{
	VerticalImageList::OnKeyDown(event);

	switch (event.GetKeyCode())
	{
	case WXK_PAGEUP:
		m_panel->ReorderSelected(true);
		break;
	case WXK_PAGEDOWN:
		m_panel->ReorderSelected(false);
		break;
	}
}

} // d2d