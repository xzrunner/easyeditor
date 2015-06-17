#include "LibraryPanel.h"

#include <wx/notebook.h>
#include <easysketch.h>

namespace eshader
{

LibraryPanel::LibraryPanel(wxWindow* parent, bool is_2d)
	: d2d::LibraryPanel(parent)
{
	wxWindow* nb = GetNotebook();
	if (is_2d) {
		AddPage(new d2d::LibraryImagePage(nb));
	} else {
		AddPage(new libsketch::LibraryPage(nb));
	}
}

}