#include "LibraryDropTarget.h"

#include "dataset/ISymbol.h"
#include "dataset/SymbolMgr.h"
#include "view/LibraryPanel.h"

namespace d2d
{

bool LibraryDropTarget::OnDropFiles(wxCoord x, wxCoord y, 
									const wxArrayString& filenames)
{
	for (int i = 0, n = filenames.size(); i < n; ++i)
	{
		wxString filename = filenames[i];
		ISymbol* symbol = SymbolMgr::Instance()->fetchSymbol(filename);
		symbol->refresh();
		m_library->AddSymbol(symbol);
		symbol->release();
	}
	return true;
}

}