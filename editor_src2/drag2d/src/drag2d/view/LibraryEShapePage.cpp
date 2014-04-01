#include "LibraryEShapePage.h"
#include "LibraryList.h"

#include "dataset/SymbolMgr.h"
#include "dataset/ISymbol.h"
#include "dataset/EShapeSymbol.h"
#include "common/FileNameParser.h"

namespace d2d
{

LibraryEShapePage::LibraryEShapePage(wxWindow* parent)
	: ILibraryPage(parent, wxT("EShape"))
{
	initLayout();
}

bool LibraryEShapePage::isHandleSymbol(ISymbol* symbol) const
{
	return dynamic_cast<EShapeSymbol*>(symbol) != NULL;
}

void LibraryEShapePage::onAddPress(wxCommandEvent& event)
{
	wxString filter = "*_" + FileNameParser::getFileTag(FileNameParser::e_shape) + ".json";
	wxFileDialog dlg(this, wxT("choose shape files"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			ISymbol* symbol = SymbolMgr::Instance()->fetchSymbol(filenames[i]);
			m_list->insert(symbol);
			symbol->release();
		}
	}
}

} // d2d