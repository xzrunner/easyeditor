#include "VerticalImageList.h"
#include "ListItem.h"

#include "dataset/Bitmap.h"
#include "widgets/DragTargetNull.h"

namespace d2d
{

static const int SPACE_UP = 5;
static const int SPACE_DOWN = 25;

VerticalImageList::VerticalImageList(wxWindow* parent, 
									 const std::string& name,
									 bool draggable /*= true*/)
	: wxVListBox(parent)
	, m_name(name)
{
	SetBackgroundColour(wxColour(229, 229, 229));
	SetSelectionBackground(wxColour(128, 128, 128));

	Connect(GetId(), wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler(VerticalImageList::OnListSelected));
	Connect(GetId(), wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler(VerticalImageList::OnListDoubleClicked));

	if (draggable)
	{
		SetDropTarget(new DragTargetNull);
		Connect(GetId(), wxEVT_MOTION, wxMouseEventHandler(VerticalImageList::OnDragInit));
	}
}

VerticalImageList::~VerticalImageList()
{
	Clear();
}

void VerticalImageList::Traverse(IVisitor& visitor) const
{
	std::vector<ListItem*>::const_iterator itr = m_items.begin();
	for ( ; itr != m_items.end(); ++itr)
	{
		bool hasNext;
		visitor.Visit(*itr, hasNext);
		if (!hasNext) break;
	}
}

void VerticalImageList::Clear()
{
	for (int i = 0, n = m_items.size(); i < n; ++i) {
		m_items[i]->Release();
	}
	m_items.clear();
	SetItemCount(0);
	Refresh(true);
}

void VerticalImageList::Insert(ListItem* item)
{
	item->Retain();
	m_items.push_back(item);
	SetItemCount(m_items.size());
	SetSelection(m_items.size() - 1);
	Refresh(true);
}

void VerticalImageList::InsertFront(ListItem* item)
{
	item->Retain();
	m_items.insert(m_items.begin(), item);
	SetItemCount(m_items.size());
	SetSelection(0);
	Refresh(true);
}

void VerticalImageList::Remove()
{
	Remove(GetSelection());
}

void VerticalImageList::Remove(int index)
{
	if (index < 0 || index >= m_items.size())
		return;

	m_items[index]->Release();
	m_items.erase(m_items.begin() + index);
 	SetItemCount(m_items.size());
	Refresh(true);
}

void VerticalImageList::Swap(int i0, int i1)
{
	if (i0 < 0 || i0 >= m_items.size() ||
		i1 < 0 || i1 >= m_items.size())
		return;

	std::swap(m_items[i0], m_items[i1]);
	Refresh(true);
}

const ListItem* VerticalImageList::GetSelected() const
{
	int idx = GetSelection();
	if (idx >= 0 && idx < m_items.size()) {
		return m_items[idx];
	} else {
		return NULL;
	}
}

void VerticalImageList::OnDrawItem(wxDC& dc, const wxRect& rect, size_t n) const
{
	bool is_selected = n == GetSelection();

	int y = rect.y + SPACE_UP;

	const Bitmap* bmp = m_items[n]->GetBitmap();
	if (bmp)
	{
		const wxBitmap* wxBmp = bmp->GetBitmap();
		if (wxBmp) 
		{
			// bmp
			int x = wxBmp->GetWidth() > rect.width ? 0 : (rect.width - wxBmp->GetWidth()) * 0.5f;
			dc.DrawBitmap(*wxBmp, x, y);

			// info
			wxString info = m_items[n]->GetInfo();
			dc.SetFont(wxFont(18, wxDEFAULT, wxNORMAL, wxNORMAL));
			//dc.SetTextForeground(wxColour(0xFF, 0x20, 0xFF));
			wxSize size = dc.GetTextExtent(info);
			//dc.DrawText(info, rect.x/* + rect.width * 0.5f - size.GetWidth() * 0.5f*/, y);
			dc.DrawText(info, rect.x + SPACE_UP, y);

			y += wxBmp->GetHeight();
		}
	}

	// name
	wxString name = m_items[n]->GetName();
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
	const Bitmap* bmp = m_items[n]->GetBitmap();
	if (bmp) {
		const wxBitmap* wxBmp = bmp->GetBitmap();
		if (wxBmp) {
			size = wxBmp->GetHeight() + SPACE_UP + SPACE_DOWN;
		}
	}
	return size;
}

void VerticalImageList::OnDragInit(wxMouseEvent& event)
{
	if (event.Dragging())
	{
		wxTextDataObject tdo(m_name + "," + wxString::FromDouble(GetSelection()));
		wxDropSource ds(tdo);
		ds.DoDragDrop(wxDrag_CopyOnly);
	}
}
} // d2d