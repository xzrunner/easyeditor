#include "StagePanel.h"
#include "LibraryPanel.h"

#include "dataset/TopPannels.h"
#include "overall/StagePanel.h"
#include "list/StagePanel.h"

namespace eui
{

BEGIN_EVENT_TABLE(StagePanel, wxPanel)
	EVT_NOTEBOOK_PAGE_CHANGED(wxID_ANY, StagePanel::OnPageChanged)
END_EVENT_TABLE()

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, TopPannels* top_pannels)
	: wxPanel(parent)
	, m_frame(frame)
	, m_top_pannels(top_pannels)
{
	InitLayout();
}

void StagePanel::Clear()
{
}

IUIStagePage* StagePanel::GetSelectedPage()
{
	if (m_pages.empty()) {
		return NULL;
	} else {
		return m_pages[m_notebook->GetSelection()];
	}
}

void StagePanel::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	InitTabPages(sizer);
	SetSizer(sizer);
}

void StagePanel::InitTabPages(wxSizer* sizer)
{
	m_notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP);
	// overall
	{
		overall::StagePanel* page = new overall::StagePanel(m_notebook, m_frame, m_top_pannels);
		m_pages.push_back(page);
		m_notebook->AddPage(page, wxT("Overall"));
	}
	// list
	{
		list::StagePanel* page = new list::StagePanel(m_notebook, m_frame, m_top_pannels);
		m_pages.push_back(page);
		m_notebook->AddPage(page, wxT("List"));
	}
	sizer->Add(m_notebook, 1, wxEXPAND);
}

void StagePanel::OnPageChanged(wxNotebookEvent& event)
{
	m_pages[m_notebook->GetSelection()]->OnSelected();
	m_top_pannels->library->Layout();
}

}