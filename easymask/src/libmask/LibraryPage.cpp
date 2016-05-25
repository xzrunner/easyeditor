#include "LibraryPage.h"
#include "config.h"

#include <ee/LibraryList.h>
#include <ee/LibraryPanel.h>
#include <ee/SymbolMgr.h>
#include <ee/Symbol.h>

namespace emask
{

LibraryPage::LibraryPage(wxWindow* parent, const std::string& name)
	: ee::LibraryPage(parent, name)
{
	InitLayout();
	m_list->SetFileter(FILE_TAG);
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("µº»Îsymbol"), wxEmptyString, 
		wxEmptyString, "*.*", wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			std::string filepath = filenames[i].ToStdString();
			ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
			symbol->RefreshThumbnail(filepath);
			m_list->Insert(symbol);
			symbol->Release();
		}
	}
}

}