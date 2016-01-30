#include "VerticalImageList.h"
#include "ListItem.h"

#include "dataset/Bitmap.h"
#include "widgets/DragTargetNull.h"

namespace d2d
{

static const int NORMAL_SPACE_UP	= 5;
static const int NORMAL_SPACE_DOWN	= 25;

static const int COMPACT_HEIGHT		= 24;
static const int COMPACT_SPACE_UP	= 4;
static const int COMPACT_SPACE_LEFT	= COMPACT_HEIGHT;

BEGIN_EVENT_TABLE(VerticalImageList, wxVListBox)
	EVT_KEY_DOWN(VerticalImageList::OnKeyDown)
	EVT_KEY_UP(VerticalImageList::OnKeyUp)
	EVT_KILL_FOCUS(VerticalImageList::OnKillFocus)
	EVT_MOTION(VerticalImageList::OnMouseEvent)
END_EVENT_TABLE()

VerticalImageList::VerticalImageList(wxWindow* parent, 
									 const std::string& name,
									 bool draggable /*= true*/,
									 bool compact/* = false*/)
	: wxVListBox(parent)
	, m_name(name)
	, m_compact(compact)
{
	SetBackgroundColour(wxColour(229, 229, 229));
	SetSelectionBackground(wxColour(128, 128, 128));

	Connect(GetId(), wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler(VerticalImageList::OnListSelected));
	Connect(GetId(), wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler(VerticalImageList::OnListDoubleClicked));

	if (draggable) {
		SetDropTarget(new DragTargetNull);
	}
}

VerticalImageList::~VerticalImageList()
{
	Clear();
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

void VerticalImageList::Insert(ListItem* item, int idx)
{
	item->Retain();
	if (idx < 0 || idx >= m_items.size()) {
		m_items.push_back(item);
		SetItemCount(m_items.size());
		SetSelection(m_items.size() - 1);
	} else {
		m_items.insert(m_items.begin() + idx, item);
		SetItemCount(m_items.size());
		SetSelection(idx);
	}
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

void VerticalImageList::Traverse(Visitor& visitor) const
{
	std::vector<ListItem*>::const_iterator itr = m_items.begin();
	for ( ; itr != m_items.end(); ++itr)
	{
		bool next;
		visitor.Visit(*itr, next);
		if (!next) break;
	}
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
	if (m_compact) 
	{
		// bmp
		if (const Bitmap* bmp = m_items[n]->GetBitmap()) {
			if (const wxBitmap* wx_bmp = bmp->GetSmallBmp()) {
				int x = COMPACT_SPACE_LEFT;
				int y = rect.y + COMPACT_SPACE_UP;
				dc.DrawBitmap(*wx_bmp, x, y);
			}
		}

		// name
		wxString name = m_items[n]->GetName();
		dc.SetFont(wxFont(is_selected ? 12 : 10, wxDEFAULT, wxNORMAL, wxNORMAL));
		wxSize size = dc.GetTextExtent(name);
		int x = rect.x + COMPACT_SPACE_LEFT + COMPACT_HEIGHT * 2;
		int y = rect.y + COMPACT_SPACE_UP;
		dc.DrawText(name, x, y);
	} 
	else 
	{
		int y = rect.y + NORMAL_SPACE_UP;
		if (const Bitmap* bmp = m_items[n]->GetBitmap()) {
			if (const wxBitmap* wx_bmp = bmp->GetLargeBmp()) {
				// bmp
				int x = wx_bmp->GetWidth() > rect.width ? 0 : (rect.width - wx_bmp->GetWidth()) * 0.5f;
				dc.DrawBitmap(*wx_bmp, x, y);

				// info
				wxString info = m_items[n]->GetInfo();
				dc.SetFont(wxFont(18, wxDEFAULT, wxNORMAL, wxNORMAL));
				//dc.SetTextForeground(wxColour(0xFF, 0x20, 0xFF));
				wxSize size = dc.GetTextExtent(info);
				//dc.DrawText(info, rect.x/* + rect.width * 0.5f - size.GetWidth() * 0.5f*/, y);
				dc.DrawText(info, rect.x + NORMAL_SPACE_UP, y);

				y += wx_bmp->GetHeight();
			}
		}

		// name
		wxString name = m_items[n]->GetName();
		dc.SetFont(wxFont(is_selected ? 12 : 10, wxDEFAULT, wxNORMAL, wxNORMAL));
		wxSize size = dc.GetTextExtent(name);
		dc.DrawText(name, rect.x + rect.width * 0.5f - size.GetWidth() * 0.5f, y + NORMAL_SPACE_UP);
	}
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
	if (m_compact) {
		return COMPACT_HEIGHT;
	} else {
		int size = NORMAL_SPACE_UP + NORMAL_SPACE_DOWN;
		if (const Bitmap* bmp = m_items[n]->GetBitmap()) {
			if (const wxBitmap* wx_bmp = bmp->GetLargeBmp()) {
				size = wx_bmp->GetHeight() + NORMAL_SPACE_UP + NORMAL_SPACE_DOWN;
			}
		}
		return size;
	}
}

void VerticalImageList::OnKeyDown(wxKeyEvent& event)
{
	wxVListBox::OnKeyDown(event);

	int key_code = event.GetKeyCode();
	m_keys_state.OnKeyDown(key_code);
}

void VerticalImageList::OnKeyUp(wxKeyEvent& event)
{
//	wxVListBox::OnKeyUp(event);

	int key_code = event.GetKeyCode();
	m_keys_state.OnKeyUp(key_code);
}

void VerticalImageList::OnKillFocus(wxFocusEvent& event)
{
	m_keys_state.Reset();
}

void VerticalImageList::OnMouseEvent(wxMouseEvent& event)
{
}

}