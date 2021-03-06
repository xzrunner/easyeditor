#include "LibraryPanel.h"

#include <ee/LibraryImagePage.h>

#include <easyanim.h>

#include <wx/notebook.h>

namespace etrail
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: ee::LibraryPanel(parent)
{
	wxWindow* nb = GetNotebook();
	AddPage(new ee::LibraryImagePage(nb));
	AddPage(new libanim::LibraryPage(nb));
}

}