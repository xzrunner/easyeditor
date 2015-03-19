#include "LibraryFontBlankList.h"
#include "FontBlankDialog.h"

#include "dataset/FontBlankSymbol.h"

namespace d2d
{

LibraryFontBlankList::LibraryFontBlankList(ILibraryPage* page)
	: LibraryList(page, wxT("fontblank"))
{
}

void LibraryFontBlankList::onListDoubleClicked(wxCommandEvent& event)
{
	FontBlankDialog dlg(this, static_cast<FontBlankSymbol*>(m_items[event.GetInt()]));
	dlg.ShowModal();
}

} // d2d