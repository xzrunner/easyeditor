#include "LibraryPanel.h"
#include "LibraryPage.h"

#include <wx/notebook.h>

#include <easyanim.h>
#include <easycomplex.h>
#include <easyscale9.h>

#include <ee/LibraryImagePage.h>

namespace eparticle2d
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: ee::LibraryPanel(parent)
{
	wxWindow* nb = GetNotebook();
	AddPage(new ee::LibraryImagePage(nb));
	AddPage(new ecomplex::LibraryPage(nb));
	AddPage(new eanim::LibraryPage(nb));
	AddPage(new escale9::LibraryPage(nb));
	AddPage(new LibraryPage(nb));
}

}