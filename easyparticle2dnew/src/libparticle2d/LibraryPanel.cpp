#include "LibraryPanel.h"

#include <wx/notebook.h>

#include <easyanim.h>
#include <easycomplex.h>
#include <easyscale9.h>

namespace eparticle2d
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: d2d::LibraryPanel(parent)
{
	addPage(new d2d::LibraryImagePage(getNotebook()));
	addPage(new ecomplex::LibraryPage(getNotebook()));
	addPage(new anim::LibraryPage(getNotebook()));
	addPage(new escale9::LibraryPage(getNotebook()));
}

}