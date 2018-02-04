#include "ee0/WxImageVList.h"
#include "ee0/WxLibraryItem.h"
#include "ee0/WxBitmap.h"

#include <set>
#include <algorithm>

namespace ee0
{

static const int NORMAL_SPACE_UP	= 5;
static const int NORMAL_SPACE_DOWN	= 25;

static const int COMPACT_HEIGHT		= 24;
static const int COMPACT_SPACE_UP	= 4;
static const int COMPACT_SPACE_LEFT	= COMPACT_HEIGHT;

BEGIN_EVENT_TABLE(WxImageVList, wxVListBox)
	EVT_KEY_DOWN(WxImageVList::OnKeyDown)
	EVT_KEY_UP(WxImageVList::OnKeyUp)
	EVT_KILL_FOCUS(WxImageVList::OnKillFocus)
	EVT_MOUSE_EVENTS(WxImageVList::OnMouseWrap)
END_EVENT_TABLE()

WxImageVList::WxImageVList(wxWindow* parent, 
									 const std::string& name,
									 bool draggable /*= true*/,
									 bool compact/* = false*/,
									 bool multiple/* = false*/)
	: wxVListBox(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, multiple ? wxLB_MULTIPLE : 0)
	, m_name(name)
	, m_compact(compact)
{
	SetBackgroundColour(wxColour(229, 229, 229));
	SetSelectionBackground(wxColour(128, 128, 128));

	Connect(GetId(), wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler(WxImageVList::OnListSelected));
	Connect(GetId(), wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler(WxImageVList::OnListDoubleClicked));

	//if (draggable) {
	//	SetDropTarget(new DragTargetNull);
	//}
}

void WxImageVList::Clear()
{
	m_items.clear();
	SetItemCount(0);
	Refresh(true);
}

void WxImageVList::Insert(const std::shared_ptr<WxLibraryItem>& item, int idx)
{
	if (idx < 0 || idx >= static_cast<int>(m_items.size())) {
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

void WxImageVList::Remove()
{
	if (HasMultipleSelection()) {
		unsigned long cookie;
		int item = GetFirstSelected(cookie);
		while (item != wxNOT_FOUND) {
			Remove(item);
			item = GetNextSelected(cookie);
		}
	} else {
		Remove(GetSelection());
	}
}

void WxImageVList::Remove(int index)
{
	if (index < 0 || index >= static_cast<int>(m_items.size()))
		return;

	m_items.erase(m_items.begin() + index);
 	SetItemCount(m_items.size());
	Refresh(true);
}

void WxImageVList::Swap(int i0, int i1)
{
	if (i0 < 0 || i0 >= static_cast<int>(m_items.size()) ||
		i1 < 0 || i1 >= static_cast<int>(m_items.size()))
		return;

	std::swap(m_items[i0], m_items[i1]);
	Refresh(true);
}

std::shared_ptr<WxLibraryItem> WxImageVList::GetSelected() const
{
	int idx = -1;
	if (HasMultipleSelection()) {
		unsigned long cookie;
		idx = GetFirstSelected(cookie);
	} else {
		idx = GetSelection();
	}
	if (idx >= 0 && idx < static_cast<int>(m_items.size())) {
		return m_items[idx];
	} else {
		return nullptr;
	}
}

void WxImageVList::OnDrawItem(wxDC& dc, const wxRect& rect, size_t n) const
{
	bool is_selected;
	if (HasMultipleSelection()) {
		std::set<int> selected;
		unsigned long cookie;
		int item = GetFirstSelected(cookie);
		while (item != wxNOT_FOUND) {
			selected.insert(item);
			item = GetNextSelected(cookie);
		}
		is_selected = selected.find(n) != selected.end();
	} else {
		is_selected = n == GetSelection();
	}

	if (m_compact) 
	{
		// bmp
		if (auto& bmp = m_items[n]->GetBitmap()) {
			if (auto& wx_bmp = bmp->GetSmallBmp()) {
				int x = COMPACT_SPACE_LEFT;
				int y = rect.y + COMPACT_SPACE_UP;
				dc.DrawBitmap(*wx_bmp, x, y);
			}
		}

		// name
		auto& name = m_items[n]->GetName();
		dc.SetFont(wxFont(is_selected ? 12 : 10, wxDEFAULT, wxNORMAL, wxNORMAL));
		wxSize size = dc.GetTextExtent(name.c_str());
		int x = rect.x + COMPACT_SPACE_LEFT + COMPACT_HEIGHT * 2;
		int y = rect.y + COMPACT_SPACE_UP;
		dc.DrawText(name.c_str(), x, y);
	} 
	else 
	{
		int y = rect.y + NORMAL_SPACE_UP;
		if (auto bmp = m_items[n]->GetBitmap()) {
			if (auto& wx_bmp = bmp->GetLargeBmp()) {
				// bmp
				int x = wx_bmp->GetWidth() > rect.width ? 0 : (rect.width - wx_bmp->GetWidth()) * 0.5f;
				dc.DrawBitmap(*wx_bmp, x, y);

				// info
				auto info = m_items[n]->GetInfo();
				dc.SetFont(wxFont(18, wxDEFAULT, wxNORMAL, wxNORMAL));
				//dc.SetTextForeground(wxColour(0xFF, 0x20, 0xFF));
				wxSize size = dc.GetTextExtent(info.c_str());
				//dc.DrawText(info, rect.x/* + rect.width * 0.5f - size.GetWidth() * 0.5f*/, y);
				dc.DrawText(info.c_str(), rect.x + NORMAL_SPACE_UP, y);

				y += wx_bmp->GetHeight();
			}
		}

		// name
		auto name = m_items[n]->GetName();
		dc.SetFont(wxFont(is_selected ? 12 : 10, wxDEFAULT, wxNORMAL, wxNORMAL));
		wxSize size = dc.GetTextExtent(name.c_str());
		dc.DrawText(name.c_str(), rect.x + rect.width * 0.5f - size.GetWidth() * 0.5f, y + NORMAL_SPACE_UP);
	}
}

void WxImageVList::OnDrawSeparator(wxDC& dc, wxRect& rect, size_t n) const
{
	dc.SetPen(wxPen(wxColour(0, 0, 0), 3));
	dc.DrawLine(rect.GetLeftBottom(), rect.GetRightBottom());
}

wxCoord WxImageVList::OnMeasureItem(size_t n) const
{
	if (m_compact) {
		return COMPACT_HEIGHT;
	} else {
		int size = NORMAL_SPACE_UP + NORMAL_SPACE_DOWN;
		if (auto& bmp = m_items[n]->GetBitmap()) {
			if (auto& wx_bmp = bmp->GetLargeBmp()) {
				size = wx_bmp->GetHeight() + NORMAL_SPACE_UP + NORMAL_SPACE_DOWN;
			}
		}
		return size;
	}
}

void WxImageVList::OnKeyDown(wxKeyEvent& event)
{
	wxVListBox::OnKeyDown(event);

		//int key_code = event.GetKeyCode();
		//m_keys_state.OnKeyDown(key_code);
}

void WxImageVList::OnKeyUp(wxKeyEvent& event)
{
	//	wxVListBox::OnKeyUp(event);

	//int key_code = event.GetKeyCode();
	//m_keys_state.OnKeyUp(key_code);
}

void WxImageVList::OnKillFocus(wxFocusEvent& event)
{
	//m_keys_state.Reset();
}

void WxImageVList::OnMouseWrap(wxMouseEvent& event)
{
	OnMouse(event);

	// The handler of this event should normally call event.Skip() to allow the default processing 
	// to take place as otherwise the window under mouse wouldn't get the focus.
	if (event.LeftDown()) {
		event.Skip();
	}
}

}