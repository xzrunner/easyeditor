#include "LibraryPanel.h"

#include <easy3d/LibraryPage.h>

#include <wx/notebook.h>

namespace eanim3d
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: ee::LibraryPanel(parent)
{
	wxWindow* nb = GetNotebook();
//	AddPage(new ee::LibraryImagePage(nb));
	AddPage(new e3d::LibraryPage(nb));
}

}