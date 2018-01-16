#include "LibraryPanel.h"
#include "LibraryPage.h"

#include <ee/LibraryImagePage.h>

#include <easynode3d/LibraryPage.h>

#include <easycomplex.h>

namespace ecomplex3d
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: ee::LibraryPanel(parent)
{
	wxWindow* nb = GetNotebook();
	AddPage(new LibraryPage(nb));
	AddPage(new enode3d::LibraryPage(nb));
	AddPage(new ee::LibraryImagePage(nb));
	AddPage(new ecomplex::LibraryPage(nb));
}

}