#include "LibraryPanel.h"

#include <ee/LibraryImagePage.h>

#include <ee3/LibGeoPage.h>
#include <ee3/LibObjPage.h>

#include <easycomplex.h>

namespace ecomplex3d
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: ee::LibraryPanel(parent)
{
	wxWindow* nb = GetNotebook();
	AddPage(new ee3::LibGeoPage(nb));
	AddPage(new ee3::LibObjPage(nb));
	AddPage(new ee::LibraryImagePage(nb));
	AddPage(new ecomplex::LibraryPage(nb));
}

}