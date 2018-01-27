#include "LibraryPanel.h"

#include <ee/LibraryImagePage.h>

#include <ee3/LibGeoPage.h>
#include <ee3/LibObjPage.h>

namespace eone
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: ee::LibraryPanel(parent)
{
	wxWindow* nb = GetNotebook();
	AddPage(new ee3::LibGeoPage(nb));
	//	AddPage(new ee::LibraryImagePage(nb));
	AddPage(new ee3::LibObjPage(nb));
}

}