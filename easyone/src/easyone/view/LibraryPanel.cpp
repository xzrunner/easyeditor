#include "view/LibraryPanel.h"
#include "view/LibGeoPage.h"

#include <ee/LibraryImagePage.h>
#include <easynode3d/LibraryPage.h>

namespace eone
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: ee::LibraryPanel(parent)
{
	wxWindow* nb = GetNotebook();
	AddPage(new LibGeoPage(nb));
	//	AddPage(new ee::LibraryImagePage(nb));
	AddPage(new enode3d::LibraryPage(nb));
}

}