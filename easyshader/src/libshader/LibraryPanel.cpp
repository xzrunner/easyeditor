#include "LibraryPanel.h"

#include <easysketch.h>

#include <ee/LibraryImagePage.h>

#include <wx/notebook.h>

namespace eshader
{

LibraryPanel::LibraryPanel(wxWindow* parent, bool is_2d)
	: ee::LibraryPanel(parent)
{
	wxWindow* nb = GetNotebook();
	if (is_2d) {
		AddPage(new ee::LibraryImagePage(nb));
	} else {
		AddPage(new esketch::LibraryPage(nb));
	}
}

}