#include "LibraryShapePage.h"
#include "LibraryList.h"

#include "dataset/SymbolMgr.h"
#include "dataset/ISymbol.h"
#include "dataset/ShapeSymbol.h"
#include "common/FileNameParser.h"

namespace d2d
{

LibraryShapePage::LibraryShapePage(wxWindow* parent)
	: ILibraryPage(parent, wxT("Shape"))
{
	initLayout();
}

bool LibraryShapePage::isHandleSymbol(ISymbol* symbol) const
{
	return dynamic_cast<ShapeSymbol*>(symbol) != NULL;
}

void LibraryShapePage::onAddPress(wxCommandEvent& event)
{
	wxString filter = "*_" + FileNameParser::getFileTag(FileNameParser::e_polygon) + ".txt";
	wxFileDialog dlg(this, wxT("choose shape files"), wxEmptyString, 
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