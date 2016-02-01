#include "LibraryPanel.h"

#include <wx/notebook.h>

#include <easytexture.h>
#include <easycomplex.h>
#include <easyshape.h>

namespace emap
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: ee::LibraryPanel(parent)
{
	wxWindow* nb = GetNotebook();
	AddPage(new ee::LibraryImagePage(nb));
	AddPage(new ecomplex::LibraryPage(nb));
	AddPage(new etexture::LibraryPage(nb));
	AddPage(new eshape::LibraryPage(nb));
}

}