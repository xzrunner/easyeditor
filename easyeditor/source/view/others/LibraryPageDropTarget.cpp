#include "LibraryPageDropTarget.h"
#include "LibraryPage.h"
#include "Symbol.h"
#include "SymbolMgr.h"

namespace ee
{

bool LibraryPageDropTarget::OnDropFiles(wxCoord x, wxCoord y, 
										const wxArrayString& filenames)
{
	for (int i = 0, n = filenames.size(); i < n; ++i)
	{
		std::string filename = filenames[i].ToStdString();
		Symbol* symbol = SymbolMgr::Instance()->FetchSymbol(filename);
		symbol->RefreshThumbnail(filename);
		if (m_page->IsHandleSymbol(symbol)) {
			m_page->AddItem(symbol);
		}
		symbol->RemoveReference();
	}
	return true;
}

}