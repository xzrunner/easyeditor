#pragma once

#include <wx/wx.h>
#include <wx/vlbox.h>

#include <vector>
#include <memory>

namespace ee0
{

class WxLibraryItem;

class WxImageVList : public wxVListBox
{
public:
	WxImageVList(wxWindow* parent, const std::string& name,
		bool draggable = true, bool compact = false, bool multiple = false);

	virtual void OnListSelected(wxCommandEvent& event) {}
	virtual void OnListDoubleClicked(wxCommandEvent& event) {}

	virtual void Clear();
	virtual void Insert(const std::shared_ptr<WxLibraryItem>& item, int idx = -1);
	virtual void Remove();
	virtual void Remove(int index);
	virtual void Swap(int i0, int i1);

	void SetFileter(const std::string& filter) {
		m_name = filter;
	}

	std::shared_ptr<WxLibraryItem> GetSelected() const;

protected:
	// wxVListBox
	virtual void OnDrawItem(wxDC& dc, const wxRect& rect, size_t n) const;
//		virtual void OnDrawBackground(wxDC& dc, const wxRect& rect, size_t n) const;
	virtual void OnDrawSeparator(wxDC& dc, wxRect& rect, size_t n) const;
	virtual wxCoord OnMeasureItem(size_t n) const;

	virtual void OnKeyDown(wxKeyEvent& event);
	virtual void OnKeyUp(wxKeyEvent& event);
	virtual void OnKillFocus(wxFocusEvent& event);
	virtual void OnMouse(wxMouseEvent& event) {}

private:
	void OnMouseWrap(wxMouseEvent& event);

protected:
	std::vector<std::shared_ptr<WxLibraryItem>> m_items;

	std::string m_name;

private:
	bool m_compact;

	DECLARE_EVENT_TABLE()

}; // WxImageVList

}