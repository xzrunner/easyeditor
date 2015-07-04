#include "LibraryPanel.h"
#include "LibraryPage.h"

#include <wx/notebook.h>

namespace eanim3d
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: d2d::LibraryPanel(parent)
{
	wxWindow* nb = GetNotebook();
//	AddPage(new d2d::LibraryImagePage(nb));
	AddPage(new LibraryPage(nb));
}

}