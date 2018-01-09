#include "LibraryPanel.h"

#include <easynode3d/LibraryPage.h>

#include <wx/notebook.h>

namespace eanim3d
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: ee::LibraryPanel(parent)
{
	wxWindow* nb = GetNotebook();
//	AddPage(new ee::LibraryImagePage(nb));
	AddPage(new enode3d::LibraryPage(nb));
}

}