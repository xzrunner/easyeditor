#include "LibraryPanel.h"

#include <wx/notebook.h>

#include <easytexture.h>

namespace emap
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: d2d::LibraryPanel(parent)
{
	wxWindow* nb = getNotebook();
	addPage(new d2d::LibraryImagePage(nb));
	addPage(new etexture::LibraryPage(nb));
}

}