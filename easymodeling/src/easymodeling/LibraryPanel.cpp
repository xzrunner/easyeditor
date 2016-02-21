#include "LibraryPanel.h"
#include "LibraryPage.h"

namespace emodeling
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: ee::LibraryPanel(parent)
{
	AddPage(new LibraryPage(GetNotebook()));
}

}