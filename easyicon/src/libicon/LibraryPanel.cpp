#include "LibraryPanel.h"

#include <ee/LibraryImagePage.h>

#include <wx/notebook.h>

namespace eicon
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: ee::LibraryPanel(parent)
{
	wxWindow* nb = GetNotebook();
	AddPage(new ee::LibraryImagePage(nb));
}

}