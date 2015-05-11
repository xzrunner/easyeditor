#include "LibraryPanel.h"
#include "LibraryPage.h"

#include <wx/notebook.h>

#include <easyanim.h>
#include <easycomplex.h>
#include <easyscale9.h>

namespace eparticle2d
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: d2d::LibraryPanel(parent)
{
	wxWindow* nb = getNotebook();
	addPage(new d2d::LibraryImagePage(nb));
	addPage(new ecomplex::LibraryPage(nb));
	addPage(new libanim::LibraryPage(nb));
	addPage(new escale9::LibraryPage(nb));
	addPage(new LibraryPage(nb));
}

}