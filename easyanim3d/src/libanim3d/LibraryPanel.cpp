#include "LibraryPanel.h"

#include <ee3/LibObjPage.h>

#include <wx/notebook.h>

namespace eanim3d
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: ee::LibraryPanel(parent)
{
	wxWindow* nb = GetNotebook();
//	AddPage(new ee::LibraryImagePage(nb));
	AddPage(new ee3::LibObjPage(nb));
}

}