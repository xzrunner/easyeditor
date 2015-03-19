#include "LibraryPanelDropTarget.h"
#include "LibraryPanel.h"

#include "dataset/ISymbol.h"
#include "dataset/SymbolMgr.h"

namespace d2d
{

bool LibraryPanelDropTarget::OnDropFiles(wxCoord x, wxCoord y, 
									const wxArrayString& filenames)
{
	for (int i = 0, n = filenames.size(); i < n; ++i)
	{
		wxString filename = filenames[i];
		ISymbol* symbol = SymbolMgr::Instance()->fetchSymbol(filename);
		symbol->RefreshThumbnail(filename);
		m_panel->AddSymbol(symbol);
		symbol->Release();
	}
	return true;
}

}