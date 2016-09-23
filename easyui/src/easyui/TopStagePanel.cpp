#include "TopStagePanel.h"
#include "TopLibraryPanel.h"
#include "TopPannels.h"
#include "TopToolbarPanel.h"
#include "Frame.h"

#include "window/StagePanel.h"
#include "list/StagePanel.h"
#include "wrapper/StagePanel.h"

#include <ee/StageCanvas.h>
#include <ee/EditPanelImpl.h>
#include <ee/FileType.h>

namespace eui
{

BEGIN_EVENT_TABLE(TopStagePanel, wxPanel)
	EVT_NOTEBOOK_PAGE_CHANGING(wxID_ANY, TopStagePanel::OnPageChanging)
	EVT_NOTEBOOK_PAGE_CHANGED(wxID_ANY, TopStagePanel::OnPageChanged)
END_EVENT_TABLE()

TopStagePanel::TopStagePanel(wxWindow* parent, Frame* frame, TopPannels* top_pannels)
	: wxPanel(parent)
	, m_frame(frame)
	, m_top_pannels(top_pannels)
{
	InitLayout();
}

UIStagePage* TopStagePanel::GetSelectedPage()
{
	if (m_pages.empty()) {
		return NULL;
	} else {
		return m_pages[m_notebook->GetSelection()];
	}
}

void TopStagePanel::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	InitTabPages(sizer);
	SetSizer(sizer);
}

void TopStagePanel::InitTabPages(wxSizer* sizer)
{
	wxGLContext* gl_ctx = NULL;

	m_notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP);
	// window
	{
		window::StagePanel* page = new window::StagePanel(m_notebook, m_frame, m_top_pannels);
		gl_ctx = page->GetCanvas()->GetGLContext();
		m_pages.push_back(page);
		m_notebook->AddPage(page, wxT("Window"));
		page->EnableObserve(true);
		page->GetStageImpl()->EnableObserve(true);
	}
	// list
	{
		list::StagePanel* page = new list::StagePanel(m_notebook, m_frame, gl_ctx, m_top_pannels);
		m_pages.push_back(page);
		m_notebook->AddPage(page, wxT("List"));
		page->EnableObserve(false);
		page->GetStageImpl()->EnableObserve(false);
	}
	// wrapper
	{
		wrapper::StagePanel* page = new wrapper::StagePanel(m_notebook, m_frame, gl_ctx, m_top_pannels);
		m_pages.push_back(page);
		m_notebook->AddPage(page, wxT("Wrapper"));
		page->EnableObserve(false);
		page->GetStageImpl()->EnableObserve(false);
	}
	m_top_pannels->toolbar->EnableToolbar(0);
	sizer->Add(m_notebook, 1, wxEXPAND);
}

void TopStagePanel::OnPageChanging(wxNotebookEvent& event)
{
	m_pages[m_notebook->GetSelection()]->EnablePage(false);
}

void TopStagePanel::OnPageChanged(wxNotebookEvent& event)
{
	int idx = m_notebook->GetSelection();
	m_pages[idx]->EnablePage(true);
	m_top_pannels->library->Layout();

	if (idx == 0) {
		m_frame->SetFileFilter(ee::FileType::GetTag(ee::FILE_UIWND));
	} else {
		m_frame->SetFileFilter(ee::FileType::GetTag(ee::FILE_UI));
	}
}

}