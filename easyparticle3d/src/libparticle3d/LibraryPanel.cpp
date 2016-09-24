#include "LibraryPanel.h"
#include "LibraryPage.h"

#include <easyanim.h>
#include <easycomplex.h>
#include <easyscale9.h>
#include <easytrail.h>

#include <ee/LibraryImagePage.h>

#include <wx/notebook.h>

namespace eparticle3d
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: ee::LibraryPanel(parent)
{
	wxWindow* nb = GetNotebook();
	AddPage(new ee::LibraryImagePage(nb));
	AddPage(new ecomplex::LibraryPage(nb));
	AddPage(new eanim::LibraryPage(nb));
	AddPage(new escale9::LibraryPage(nb));
	AddPage(new etrail::LibraryPage(nb));
	AddPage(new LibraryPage(nb));
}

}