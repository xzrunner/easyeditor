#include "LibraryPanel.h"
#include "LibraryPage.h"

#include <ee/LibraryImagePage.h>

#include <easycomplex.h>

namespace ecomplex3d
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: ee::LibraryPanel(parent)
{
	wxWindow* nb = GetNotebook();
	AddPage(new ee::LibraryImagePage(nb));
	AddPage(new ecomplex::LibraryPage(nb));
	AddPage(new LibraryPage(nb));
}

}