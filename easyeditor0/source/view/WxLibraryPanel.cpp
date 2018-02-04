#include "ee0/WxLibraryPanel.h"
#include "ee0/WxLibraryPage.h"

namespace ee0
{

WxLibraryPanel::WxLibraryPanel(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
	, m_notebook(nullptr)
	, m_selected(nullptr)
{
	InitLayout();
}

void WxLibraryPanel::AddPage(WxLibraryPage* page, const char* name)
{
	if (m_notebook->GetPageCount() == 0)
		m_selected = page;

	if (name != NULL) {
		m_notebook->AddPage(page, name);
	} else {
		m_notebook->AddPage(page, page->GetPageName().c_str());
	}
	m_pages.push_back(page);
}

std::shared_ptr<WxLibraryItem> WxLibraryPanel::GetItem(int idx) const
{
	if (m_selected) {
		return m_selected->GetItem(idx);
	} else {
		return nullptr;
	}
}

void WxLibraryPanel::InitLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	m_notebook = new wxNotebook(this, wxID_ANY);
	Connect(m_notebook->GetId(), wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, 
		wxBookCtrlEventHandler(WxLibraryPanel::OnPageChanged));
	Connect(m_notebook->GetId(), wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING, 
		wxBookCtrlEventHandler(WxLibraryPanel::OnPageChanging));

	sizer->Add(m_notebook, 1, wxEXPAND);

	SetSizer(sizer);
}

void WxLibraryPanel::OnPageChanged(wxBookCtrlEvent& event)
{
	m_selected = m_pages[event.GetSelection()];
//	m_selected->OnActive();
}

void WxLibraryPanel::OnPageChanging(wxBookCtrlEvent& event)
{
}

}