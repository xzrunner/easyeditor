#include "LibraryFontBlankList.h"
#include "FontBlankDialog.h"
#include "FontBlankSymbol.h"

namespace ee
{

LibraryFontBlankList::LibraryFontBlankList(LibraryPage* page)
	: LibraryList(page, "fontblank")
{
}

void LibraryFontBlankList::OnListDoubleClicked(wxCommandEvent& event)
{
	FontBlankDialog dlg(this, std::dynamic_pointer_cast<FontBlankSymbol>(m_items[event.GetInt()]));
	dlg.ShowModal();
}

}