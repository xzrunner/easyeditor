#include "VerticalImageList.h"
#include "ListItem.h"

#include "dataset/Bitmap.h"
#include "widgets/DragTargetNull.h"

namespace d2d
{

VerticalImageList::VerticalImageList(wxWindow* parent, const wxString& name/* = wxEmptyString*/,
									 bool draggable /*= true*/)
	: wxVListBox(parent)
	, m_name(name)
{
	SetBackgroundColour(wxColour(229, 229, 229));
	SetSelectionBackground(wxColour(128, 128, 128));

	Connect(GetId(), wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler(VerticalImageList::onListSelected));
	Connect(GetId(), wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler(VerticalImageList::onListDoubleClicked));

	if (draggable)
	{
		SetDropTarget(new DragTargetNull);
		Connect(GetId(), wxEVT_MOTION, wxMouseEventHandler(VerticalImageList::onDragInit));
	}
}

VerticalImageList::~VerticalImageList()
{
	clear();
}

void VerticalImageList::traverse(IVisitor& visitor) const
{
	std::vector<ListItem*>::const_iterator itr = m_items.begin();
	for ( ; itr != m_items.end(); ++itr)
	{
		bool hasNext;
		visitor.Visit(*itr, hasNext);
		if (!hasNext) break;
	}
}

void VerticalImageList::clear()
{
	for (int i = 0, n = m_items.size(); i < n; ++i) {
		m_items[i]->Release();
	}
	m_items.clear();
	SetItemCount(0);
	Refresh();
}

void VerticalImageList::insert(ListItem* item)
{
	item->Retain();
	m_items.push_back(item);
	SetItemCount(m_items.size());
	SetSelection(m_items.size() - 1);
	Refresh();
}

void VerticalImageList::insertFront(ListItem* item)
{
	item->Retain();
	m_items.insert(m_items.begin(), item);
	SetItemCount(m_items.size());
	SetSelection(0);
	Refresh();
}

void VerticalImageList::remove()
{
	remove(GetSelection());
}

void VerticalImageList::remove(int index)
{
	if (index < 0 || index >= m_items.size())
		return;

	m_items[index]->Release();
	m_items.erase(m_items.begin() + index);
 	SetItemCount(m_items.size());
	Refresh();
}

void VerticalImageList::swap(int i0, int i1)
{
	if (i0 < 0 || i0 >= m_items.size() ||
		i1 < 0 || i1 >= m_items.size())
		return;

	ListItem* tmp = m_items[i0];
	m_items[i0] = m_items[i1];
	m_items[i1] = tmp;

	Refresh();
}

void VerticalImageList::OnDrawItem(wxDC& dc, const wxRect& rect, size_t n) const
{
	bool is_selected = n == GetSelection();

	int y = rect.y + SPACE_UP;

	const Bitmap* bmp = m_items[n]->getBitmap();
	if (bmp)
	{
		const wxBitmap* wxBmp = bmp->GetBitmap();
		if (wxBmp) 
		{
			// bmp
			int x = wxBmp->GetWidth() > rect.width ? 0 : (rect.width - wxBmp->GetWidth()) * 0.5f;
			dc.DrawBitmap(*wxBmp, x, y);

			// info
			wxString info = m_items[n]->getInfo();
			dc.SetFont(wxFont(18, wxDEFAULT, wxNORMAL, wxNORMAL));
			//dc.SetTextForeground(wxColour(0xFF, 0x20, 0xFF));
			wxSize size = dc.GetTextExtent(info);
			//dc.DrawText(info, rect.x/* + rect.width * 0.5f - size.GetWidth() * 0.5f*/, y);
			dc.DrawText(info, rect.x + SPACE_UP, y);

			y += wxBmp->GetHeight();
		}
	}

	// name
	wxString name = m_items[n]->getName();
	dc.SetFont(wxFont(is_selected ? 12 : 10, wxDEFAULT, wxNORMAL, wxNORMAL));
	wxSize size = dc.GetTextExtent(name);
	dc.DrawText(name, rect.x + rect.width * 0.5f - size.GetWidth() * 0.5f, y + SPACE_UP);
}

// void VerticalImageList::OnDrawBackground(wxDC& dc, const wxRect& rect, size_t n) const
// {
//   	dc.SetPen(*wxRED_PEN);
//   	dc.DrawRectangle(rect);
// }

void VerticalImageList::OnDrawSeparator(wxDC& dc, wxRect& rect, size_t n) const
{
	dc.SetPen(wxPen(wxColour(0, 0, 0), 3));
	dc.DrawLine(rect.GetLeftBottom(), rect.GetRightBottom());
}

wxCoord VerticalImageList::OnMeasureItem(size_t n) const
{
	int size = SPACE_UP + SPACE_DOWN;
	const Bitmap* bmp = m_items[n]->getBitmap();
	if (bmp) {
		const wxBitmap* wxBmp = bmp->GetBitmap();
		if (wxBmp) {
			size = wxBmp->GetHeight() + SPACE_UP + SPACE_DOWN;
		}
	}
	return size;
}

void VerticalImageList::onDragInit(wxMouseEvent& event)
{
	if (event.Dragging())
	{
		wxTextDataObject tdo(m_name + "," + wxString::FromDouble(GetSelection()));
		wxDropSource ds(tdo);
		ds.DoDragDrop(wxDrag_CopyOnly);
	}
}
} // d2d