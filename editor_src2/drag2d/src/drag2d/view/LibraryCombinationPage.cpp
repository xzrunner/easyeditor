#include "LibraryCombinationPage.h"
#include "LibraryList.h"

#include "dataset/SymbolMgr.h"
#include "dataset/CombinationSymbol.h"
#include "common/FileNameParser.h"

namespace d2d
{

LibraryCombinationPage::LibraryCombinationPage(wxWindow* parent)
	: ILibraryPage(parent, wxT("组合"))
{
	initLayout();
}

bool LibraryCombinationPage::isHandleSymbol(ISymbol* symbol) const
{
	return dynamic_cast<CombinationSymbol*>(symbol) != NULL;
}

void LibraryCombinationPage::onAddPress(wxCommandEvent& event)
{
	wxString filter = wxT("*_") + FileNameParser::getFileTag(FileNameParser::e_combination) + wxT(".txt");
	wxFileDialog dlg(this, wxT("导入组合文件"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			ISymbol* symbol = SymbolMgr::Instance()->getSymbol(filenames[i]);
			m_list->insert(symbol);
		}
	}
}

} // d2d