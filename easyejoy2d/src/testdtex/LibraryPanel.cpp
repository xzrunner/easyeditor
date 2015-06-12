#include "LibraryPanel.h"

#include <wx/notebook.h>

namespace tdtex
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: d2d::LibraryPanel(parent)
{
	wxWindow* nb = GetNotebook();
	AddPage(new d2d::LibraryImagePage(nb));
}

}