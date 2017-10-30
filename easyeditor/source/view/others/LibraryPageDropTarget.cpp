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
		auto sym = SymbolMgr::Instance()->FetchSymbol(filename);
		sym->RefreshThumbnail(filename);
		if (m_page->IsHandleSymbol(sym)) {
			m_page->AddItem(sym);
		}
	}
	return true;
}

}