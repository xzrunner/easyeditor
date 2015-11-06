#include "LibraryPanel.h"

#include <wx/notebook.h>

namespace tgtxt
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: d2d::LibraryPanel(parent)
{
	wxWindow* nb = GetNotebook();
	AddPage(new d2d::LibraryImagePage(nb));
}

}