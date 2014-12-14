#include "LibraryPanel.h"

#include <wx/notebook.h>
#include <easysketch.h>

namespace eshader
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: d2d::LibraryPanel(parent)
{
	wxWindow* nb = getNotebook();
	addPage(new d2d::LibraryImagePage(nb));
	addPage(new libsketch::LibraryPage(nb));
}

}