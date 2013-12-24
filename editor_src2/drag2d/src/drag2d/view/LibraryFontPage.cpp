#include "LibraryFontPage.h"
#include "LibraryList.h"

#include "dataset/SymbolMgr.h"
#include "dataset/FontSymbol.h"

namespace d2d
{

LibraryFontPage::LibraryFontPage(wxWindow* parent)
	: ILibraryPage(parent, wxT("Font"))
{
	initLayout();
}

bool LibraryFontPage::isHandleSymbol(ISymbol* symbol) const
{
	return dynamic_cast<FontSymbol*>(symbol) != NULL;
}

void LibraryFontPage::onAddPress(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Choose Font"), wxEmptyString, 
		wxEmptyString, wxT("*.ttf"), wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		ISymbol* symbol = SymbolMgr::Instance()->getSymbol(dlg.GetPath());
		m_list->insert(symbol);
	}
}
} // d2d