#include "LibraryPanel.h"

#include <wx/notebook.h>

namespace epseudo3d
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: ee::LibraryPanel(parent)
{
	wxWindow* nb = GetNotebook();
	AddPage(new ee::LibraryImagePage(nb));
}

}