#pragma once

#include <wx/wx.h>
#include <wx/notebook.h>

#include <memory>

namespace ee0
{

class WxLibraryPage;
class WxLibraryItem;

class WxLibraryPanel : public wxPanel
{
public:
	WxLibraryPanel(wxWindow* parent);

	void AddPage(WxLibraryPage* page, const char* name = nullptr);

	wxWindow* GetNotebook() { return m_notebook; }

	std::shared_ptr<WxLibraryItem> GetItem(int idx = -1) const;

private:
	void InitLayout();

	void OnPageChanged(wxBookCtrlEvent& event);
	void OnPageChanging(wxBookCtrlEvent& event);

private:
	wxNotebook* m_notebook;

	std::vector<WxLibraryPage*> m_pages;

	WxLibraryPage* m_selected;

}; // WxLibraryPanel

}