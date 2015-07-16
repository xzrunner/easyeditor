#include "LibraryList.h"
#include "ILibraryPage.h"
#include "LibraryPageDropTarget.h"

#include "common/StringTools.h"
#include "dataset/ISymbol.h"

namespace d2d
{

LibraryList::LibraryList(ILibraryPage* page,
						 const std::string& filter/* = wxT("symbol")*/,
						 bool draggable /*= true*/)
	: VerticalImageList(page, filter, draggable)
{
	SetDropTarget(new LibraryPageDropTarget(page));
}

void LibraryList::OnListSelected(wxCommandEvent& event)
{
	int idx = event.GetInt();
	m_selection_set.insert(idx);
}

void LibraryList::Clear()
{
	VerticalImageList::Clear();
	m_selection_set.clear();
}

void LibraryList::Insert(ListItem* item)
{
	VerticalImageList::Insert(item);
	m_selection_set.clear();
}

void LibraryList::InsertFront(ListItem* item)
{
	VerticalImageList::InsertFront(item);
	m_selection_set.clear();
}

void LibraryList::Remove()
{
	VerticalImageList::Remove();
	m_selection_set.clear();
}

void LibraryList::Remove(int index)
{
	VerticalImageList::Remove(index);
	m_selection_set.clear();
}

void LibraryList::Swap(int i0, int i1)
{
	VerticalImageList::Swap(i0, i1);
	m_selection_set.clear();
}

ListItem* LibraryList::GetItem(int index/* = -1*/) const
{
	if (index == -1)
		index = GetSelection();

	if (index < 0 || index >= (int)m_items.size()) 
		return NULL;
	else 
		return m_items[index];
}

void LibraryList::ReloadTexture() const
{
	for (size_t i = 0, n = m_items.size(); i < n; ++i)
		static_cast<ISymbol*>(m_items[i])->ReloadTexture();
}

void LibraryList::OnKeyDown(wxKeyEvent& event)
{
	VerticalImageList::OnKeyDown(event);

	switch (event.GetKeyCode())
	{
	case WXK_DELETE:
		Remove();
		break;
	}
}

void LibraryList::OnKillFocus(wxFocusEvent& event)
{
	VerticalImageList::OnKillFocus(event);
	m_selection_set.clear();
}

void LibraryList::OnMouseEvent(wxMouseEvent& event)
{
	VerticalImageList::OnMouseEvent(event);

	if (!event.Dragging()) {
		return;
	}

	m_selection_set.insert(GetSelection());

	std::string text = m_name + ",";
	std::set<int>::iterator itr = m_selection_set.begin();
	for ( ; itr != m_selection_set.end(); ++itr) {
		text += StringTools::IntToString(*itr) + ",";
	}

	wxTextDataObject tdo(text);
	wxDropSource ds(tdo);
	ds.DoDragDrop(wxDrag_CopyOnly);
}

} // d2d