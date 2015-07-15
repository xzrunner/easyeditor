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

void ViewlistList::OnListSelected(wxCommandEvent& event)
{
	m_panel->OnSelected(event.GetInt());
}

void ViewlistList::OnListDoubleClicked(wxCommandEvent& event)
{
	m_panel->OnSelected(event.GetInt());
}

void ViewlistList::OnKeyDown(wxKeyEvent& event)
{
	int curr_idx = this->GetSelection();
	VerticalImageList::OnKeyDown(event);

	int key_code = event.GetKeyCode();
	m_keys_state.OnKeyDown(key_code);
	switch (key_code)
	{
	case WXK_PAGEUP:
		SetSelection(curr_idx);
		m_panel->OnSelected(curr_idx);
		m_panel->ReorderSelected(true);
		break;
	case WXK_PAGEDOWN:
		SetSelection(curr_idx);
		m_panel->OnSelected(curr_idx);
		m_panel->ReorderSelected(false);
		break;
	case WXK_DELETE:
		m_panel->RemoveSelected();
		break;
	}
}

void ViewlistList::OnKeyUp(wxKeyEvent& event)
{
//	VerticalImageList::OnKeyUp(event);
	
	int key_code = event.GetKeyCode();
	m_keys_state.OnKeyUp(key_code);
}

} // d2d