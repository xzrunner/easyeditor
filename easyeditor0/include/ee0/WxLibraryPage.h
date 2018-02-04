#pragma once

#include <wx/wx.h>

#include <memory>

namespace ee0
{

class WxLibraryList;
class WxLibraryItem;

class WxLibraryPage : public wxWindow
{
public:
	WxLibraryPage(wxWindow* parent, const std::string& name, bool is_static = false);

	const std::string& GetPageName() const { return m_name; }

	std::shared_ptr<WxLibraryItem> GetItem(int idx = -1) const;

protected:
	virtual void OnAddPress(wxCommandEvent& event);
	virtual void OnDelPress(wxCommandEvent& event);

private:
	void InitLayout();
	void InitButtons(wxSizer* sizer);

protected:
	WxLibraryList* m_list;

private:
	std::string m_name;

	bool m_is_static;

	wxButton *m_btn_add, *m_btn_del;

}; // WxLibraryPage

}