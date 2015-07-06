#include "LibraryPageDropTarget.h"
#include "ILibraryPage.h"

#include "dataset/ISymbol.h"
#include "dataset/SymbolMgr.h"

namespace d2d
{

bool LibraryPageDropTarget::OnDropFiles(wxCoord x, wxCoord y, 
										const wxArrayString& filenames)
{
	for (int i = 0, n = filenames.size(); i < n; ++i)
	{
		std::string filename = filenames[i].ToStdString();
		ISymbol* symbol = SymbolMgr::Instance()->FetchSymbol(filename);
		symbol->RefreshThumbnail(filename);
		if (m_page->IsHandleSymbol(symbol)) {
			m_page->AddItem(symbol);
		}
		symbol->Release();
	}
	return true;
}

}