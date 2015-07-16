#ifndef _DRAG2D_VERTICAL_IMAGE_LIST_H_
#define _DRAG2D_VERTICAL_IMAGE_LIST_H_

#include <wx/wx.h>
#include <wx/vlbox.h>
#include <vector>

#include "view/KeysState.h"

namespace d2d
{

class ListItem;
class IVisitor;

class VerticalImageList : public wxVListBox
{
public:
	VerticalImageList(wxWindow* parent, const std::string& name,
		bool draggable = true);
	virtual ~VerticalImageList();

	// VerticalImageList
	virtual void OnListSelected(wxCommandEvent& event) {}
	virtual void OnListDoubleClicked(wxCommandEvent& event) {}

	void Traverse(IVisitor& visitor) const;

	void Clear();
	void Insert(ListItem* item);
	void InsertFront(ListItem* item);
	void Remove();
	void Remove(int index);
	void Swap(int i0, int i1);

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

private:
	void OnDragInit(wxMouseEvent& event);

protected:
	std::vector<ListItem*> m_items;

private:
	std::string m_name;

	KeysState m_keys_state;

	DECLARE_EVENT_TABLE()

}; // VerticalImageList

}

#endif // _DRAG2D_VERTICAL_IMAGE_LIST_H_