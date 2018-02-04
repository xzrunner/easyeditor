#include "LibraryPanel.h"

#include <ee0/WxLibraryImagePage.h>
#include <ee3/WxLibGeoPage.h>
#include <ee3/WxLibObjPage.h>

namespace eone
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: ee0::WxLibraryPanel(parent)
{
	wxWindow* nb = GetNotebook();
	AddPage(new ee0::WxLibraryImagePage(nb));
	AddPage(new ee3::WxLibGeoPage(nb));
	AddPage(new ee3::WxLibObjPage(nb));
}

}