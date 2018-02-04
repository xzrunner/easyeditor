#include "ee0/WxLibraryList.h"

#include <wx/dnd.h>

namespace ee0
{

WxLibraryList::WxLibraryList(wxWindow* parent, const std::string& name)
	: WxImageVList(parent, name)
{
}

void WxLibraryList::OnListSelected(wxCommandEvent& event)
{
	int idx = event.GetInt();

	if (!GetKeyState(WXK_CONTROL)) {
		m_selection_set.clear();
	}
	m_selection_set.insert(idx);
}

void WxLibraryList::Clear()
{
	WxImageVList::Clear();
	m_selection_set.clear();
}

void WxLibraryList::Insert(const std::shared_ptr<WxLibraryItem>& item, int idx)
{
	WxImageVList::Insert(item, idx);
	m_selection_set.clear();
}

void WxLibraryList::Remove()
{
	WxImageVList::Remove();
	m_selection_set.clear();
}

void WxLibraryList::Remove(int index)
{
	WxImageVList::Remove(index);
	m_selection_set.clear();
}

void WxLibraryList::Swap(int i0, int i1)
{
	WxImageVList::Swap(i0, i1);
	m_selection_set.clear();
}

std::shared_ptr<WxLibraryItem> WxLibraryList::GetItem(int index/* = -1*/) const
{
	if (index == -1)
		index = GetSelection();

	if (index < 0 || index >= (int)m_items.size())
		return nullptr;
	else
		return m_items[index];
}

void WxLibraryList::OnKeyDown(wxKeyEvent& event)
{
	WxImageVList::OnKeyDown(event);

	switch (event.GetKeyCode())
	{
	case WXK_DELETE:
		Remove();
		break;
	}
}

void WxLibraryList::OnKillFocus(wxFocusEvent& event)
{
	WxImageVList::OnKillFocus(event);
	m_selection_set.clear();
}

void WxLibraryList::OnMouse(wxMouseEvent& event)
{
	if (!event.Dragging()) {
		return;
	}

	if (!GetKeyState(WXK_CONTROL)) {
		m_selection_set.clear();
	}
	m_selection_set.insert(GetSelection());

	std::string text = m_name + ",";
	std::set<int>::iterator itr = m_selection_set.begin();
	for ( ; itr != m_selection_set.end(); ++itr) {
		text += std::to_string(*itr) + ",";
	}

	wxTextDataObject tdo(text.c_str());
	wxDropSource ds(tdo);
	ds.DoDragDrop(wxDrag_CopyOnly);
}

}