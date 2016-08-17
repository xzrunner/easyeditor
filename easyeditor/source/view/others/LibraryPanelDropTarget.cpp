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
		Symbol* sym = SymbolMgr::Instance()->FetchSymbol(filename);
		sym->RefreshThumbnail(filename);
		m_panel->AddSymbol(sym);
		sym->RemoveReference();
	}
	return true;
}

}