#include "LibraryFontBlankList.h"
#include "FontBlankDialog.h"

#include "dataset/FontBlankSymbol.h"

namespace d2d
{

LibraryFontBlankList::LibraryFontBlankList(wxWindow* parent)
	: LibraryList(parent, wxT("fontblank"))
{
}

void LibraryFontBlankList::onListDoubleClicked(wxCommandEvent& event)
{
	FontBlankDialog dlg(this, static_cast<FontBlankSymbol*>(m_items[event.GetInt()]));
	dlg.ShowModal();
	// resetCanvas
}

} // d2d