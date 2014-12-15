#include "LibraryPanel.h"

#include <wx/notebook.h>
#include <easysketch.h>

namespace eshader
{

LibraryPanel::LibraryPanel(wxWindow* parent, bool is_2d)
	: d2d::LibraryPanel(parent)
{
	wxWindow* nb = getNotebook();
	if (is_2d) {
		addPage(new d2d::LibraryImagePage(nb));
	} else {
		addPage(new libsketch::LibraryPage(nb));
	}
}

}