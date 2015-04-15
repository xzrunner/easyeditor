#include "LibraryPanel.h"

#include <wx/notebook.h>

#include <easytexture.h>
#include <easycomplex.h>
#include <easyshape.h>

namespace emap
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: d2d::LibraryPanel(parent)
{
	wxWindow* nb = getNotebook();
	addPage(new d2d::LibraryImagePage(nb));
	addPage(new ecomplex::LibraryPage(nb));
	addPage(new etexture::LibraryPage(nb));
	addPage(new libshape::LibraryPage(nb));
}

}