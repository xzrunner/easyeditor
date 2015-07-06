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
		std::string filename = filenames[i].ToStdString();
		ISymbol* symbol = SymbolMgr::Instance()->FetchSymbol(filename);
		symbol->RefreshThumbnail(filename);
		m_panel->AddSymbol(symbol);
		symbol->Release();
	}
	return true;
}

}