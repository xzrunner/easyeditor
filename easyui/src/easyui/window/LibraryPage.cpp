#include "LibraryPage.h"

#include <easycomplex.h>

namespace eui
{
namespace window
{

LibraryPage::LibraryPage(wxWindow* parent, const char* name)
	: d2d::ILibraryPage(parent, name)
{
	InitLayout();
}

bool LibraryPage::IsHandleSymbol(d2d::Symbol* symbol) const
{
// 	return dynamic_cast<d2d::ImageSymbol*>(symbol) != NULL
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
			d2d::Symbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
			symbol->RefreshThumbnail(filepath);
			m_list->Insert(symbol);
			symbol->Release();
		}
	}
}

}
}