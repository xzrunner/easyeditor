#include "TopLibraryPanel.h"

#include <ee/LibraryImagePage.h>
#include <ee/LibraryScriptsPage.h>

#include <easycomplex.h>
#include <easytext.h>

namespace eui
{

TopLibraryPanel::TopLibraryPanel(wxWindow* parent)
	: wxPanel(parent)
{
	InitLayout();
	EnableUILibrary(true);

	m_library_raw->LoadFromConfig();
}

void TopLibraryPanel::EnableUILibrary(bool enable)
{
	wxSizer* sizer = GetSizer();
	if (enable) {
		sizer->Show((size_t)0);
		sizer->Hide((size_t)1);
	} else {
		sizer->Show((size_t)1);
		sizer->Hide((size_t)0);
	}
}

void TopLibraryPanel::Clear()
{
	m_library_ui->Clear();
	m_library_raw->Clear();
}

void TopLibraryPanel::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	m_library_ui = new ee::LibraryPanel(this);
	sizer->Add(m_library_ui, 1, wxEXPAND);

	m_library_raw = new ee::LibraryPanel(this);
	wxWindow* nb = m_library_raw->GetNotebook();
	m_library_raw->AddPage(new ee::LibraryImagePage(nb));
	m_library_raw->AddPage(new ecomplex::LibraryPage(nb));
	m_library_raw->AddPage(new etext::LibraryPage(nb));
	m_library_raw->AddPage(new ee::LibraryScriptsPage(nb));
	sizer->Add(m_library_raw, 1, wxEXPAND);

	SetSizer(sizer);

	Layout();
}

}