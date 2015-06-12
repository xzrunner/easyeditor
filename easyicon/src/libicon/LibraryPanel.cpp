#include "LibraryPanel.h"

#include <wx/notebook.h>

namespace eicon
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: d2d::LibraryPanel(parent)
{
	wxWindow* nb = GetNotebook();
	AddPage(new d2d::LibraryImagePage(nb));
}

}