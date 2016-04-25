#ifndef _EASYEDITOR_VERTICAL_IMAGE_LIST_H_
#define _EASYEDITOR_VERTICAL_IMAGE_LIST_H_

#include "KeysState.h"

#include <wx/wx.h>
#include <wx/vlbox.h>

#include <vector>

namespace ee
{

class ListItem;
class Visitor;

class VerticalImageList : public wxVListBox
{
public:
	VerticalImageList(wxWindow* parent, const std::string& name,
		bool draggable = true, bool compact = false, bool multiple = false);
	virtual ~VerticalImageList();

	virtual void OnListSelected(wxCommandEvent& event) {}
	virtual void OnListDoubleClicked(wxCommandEvent& event) {}

	virtual void Clear();
	virtual void Insert(ListItem* item, int idx = -1);
	virtual void Remove();
	virtual void Remove(int index);
	virtual void Swap(int i0, int i1);

	void Traverse(Visitor& visitor) const;

	void SetFileter(const std::string& filter) {
		m_name = filter;
	}

	const ListItem* GetSelected() const;

	bool GetKeyState(int key) const {
		return m_keys_state.GetKeyState(key);
	}

protected:
	// wxVListBox
	virtual void OnDrawItem(wxDC& dc, const wxRect& rect, size_t n) const;
//		virtual void OnDrawBackground(wxDC& dc, const wxRect& rect, size_t n) const;
	virtual void OnDrawSeparator(wxDC& dc, wxRect& rect, size_t n) const;
	virtual wxCoord OnMeasureItem(size_t n) const;

	virtual void OnKeyDown(wxKeyEvent& event);
	virtual void OnKeyUp(wxKeyEvent& event);
	virtual void OnKillFocus(wxFocusEvent& event);
	virtual void OnMouseEvent(wxMouseEvent& event);

protected:
	std::vector<ListItem*> m_items;

	std::string m_name;

private:
	bool m_compact;

	KeysState m_keys_state;

	DECLARE_EVENT_TABLE()

}; // VerticalImageList

}

#endif // _EASYEDITOR_VERTICAL_IMAGE_LIST_H_