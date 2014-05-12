
#include "LibraryPanel.h"
#include "LibraryPage.h"

using namespace emodeling;

LibraryPanel::LibraryPanel(wxWindow* parent)
	: d2d::LibraryPanel(parent)
{
	addPage(new LibraryPage(getNotebook()));
}