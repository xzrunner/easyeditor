#include "LibraryPanelDropTarget.h"
#include "LibraryPanel.h"
#include "Symbol.h"
#include "SymbolMgr.h"

namespace ee
{

bool LibraryPanelDropTarget::OnDropFiles(wxCoord x, wxCoord y, 
									const wxArrayString& filenames)
{
	for (int i = 0, n = filenames.size(); i < n; ++i)
	{
		std::string filename = filenames[i].ToStdString();
		Symbol* symbol = SymbolMgr::Instance()->FetchSymbol(filename);
		symbol->RefreshThumbnail(filename);
		m_panel->AddSymbol(symbol);
		symbol->RemoveReference();
	}
	return true;
}

}