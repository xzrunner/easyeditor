#include "LibraryPanel.h"
#include "LibraryPage.h"

#include <wx/notebook.h>

#include <easyanim.h>
#include <easycomplex.h>
#include <easyscale9.h>

namespace eparticle3d
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: d2d::LibraryPanel(parent)
{
	wxWindow* nb = GetNotebook();
	AddPage(new d2d::LibraryImagePage(nb));
	AddPage(new ecomplex::LibraryPage(nb));
	AddPage(new libanim::LibraryPage(nb));
	AddPage(new escale9::LibraryPage(nb));
}

}