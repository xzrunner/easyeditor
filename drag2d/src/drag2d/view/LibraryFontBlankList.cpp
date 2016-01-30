#include "LibraryFontBlankList.h"
#include "FontBlankDialog.h"

#include "dataset/FontBlankSymbol.h"

namespace d2d
{

LibraryFontBlankList::LibraryFontBlankList(ILibraryPage* page)
	: LibraryList(page, "fontblank")
{
}

void LibraryFontBlankList::OnListDoubleClicked(wxCommandEvent& event)
{
	FontBlankDialog dlg(this, static_cast<FontBlankSymbol*>(m_items[event.GetInt()]));
	dlg.ShowModal();
}

}