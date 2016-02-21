#include "LibraryPage.h"

#include <ee/SymbolMgr.h>
#include <ee/LibraryList.h>

#include <easycomplex.h>

namespace eui
{
namespace window
{

LibraryPage::LibraryPage(wxWindow* parent, const char* name)
	: ee::LibraryPage(parent, name)
{
	InitLayout();
}

bool LibraryPage::IsHandleSymbol(ee::Symbol* symbol) const
{
// 	return dynamic_cast<ee::ImageSymbol*>(symbol) != NULL
// 		|| dynamic_cast<ecomplex::Symbol*>(symbol) != NULL;

	return true;
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
}