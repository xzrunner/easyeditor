#include "LibrarySymbolPage.h"
#include "LibrarySymbolList.h"

#include "dataset/Symbol.h"

namespace eanim
{

LibrarySymbolPage::LibrarySymbolPage(wxWindow* parent)
	: ee::ILibraryPage(parent, "Symbol")
{
	initLayout();

	wxSizer* sizer = GetSizer();
	m_list = new LibrarySymbolList(this);
	sizer->Add(m_list, 1, wxEXPAND);
	SetSizer(sizer);
}

bool LibrarySymbolPage::isHandleSymbol(ee::ISymbol* symbol) const
{
	return dynamic_cast<Symbol*>(symbol) != NULL;
}

void LibrarySymbolPage::onAddPress(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Please choose a image"), wxEmptyString, wxEmptyString, wxT("*.png;*.jpg"), wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
			m_list->insert(new Symbol(filenames[i]));
	}
}

} // eanim