#include "LibraryPanel.h"
#include "LibraryPage.h"

#include <wx/notebook.h>

namespace eanim3d
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: ee::LibraryPanel(parent)
{
	wxWindow* nb = GetNotebook();
//	AddPage(new ee::LibraryImagePage(nb));
	AddPage(new LibraryPage(nb));
}

}