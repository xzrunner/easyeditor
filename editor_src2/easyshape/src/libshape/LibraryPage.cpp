#include "LibraryPage.h"
#include "Symbol.h"

namespace libshape
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ILibraryPage(parent, wxT("Shape"))
{
	initLayout();
}

bool LibraryPage::isHandleSymbol(d2d::ISymbol* symbol) const
{
	return dynamic_cast<Symbol*>(symbol) != NULL;
}

void LibraryPage::onAddPress(wxCommandEvent& event)
{
	wxString filter = "*_" + d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_shape) + ".json";
	wxFileDialog dlg(this, wxT("choose shape files"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filenames[i]);
			m_list->insert(symbol);
			symbol->release();
		}
	}
}

}