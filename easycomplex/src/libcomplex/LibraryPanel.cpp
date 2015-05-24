#include "LibraryPanel.h"
#include "LibraryPage.h"

#include <wx/notebook.h>

#include <easyanim.h>
#include <easymesh.h>
#include <easyscale9.h>
#include <easyparticle3d.h>
#include <easyparticle2d.h>
#include <easyejoy2d.h>
#include <easyterrain2d.h>
#include <easytexture.h>
#include <easyicon.h>

namespace ecomplex
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: d2d::LibraryPanel(parent)
{
	wxWindow* nb = getNotebook();
	addPage(new d2d::LibraryImagePage(nb));
	addPage(new LibraryPage(nb));
	addPage(new libanim::LibraryPage(nb));
	addPage(new escale9::LibraryPage(nb));
	addPage(new d2d::LibraryFontBlankPage(nb));
	addPage(new emesh::LibraryPage(nb));
	addPage(new d2d::LibraryScriptsPage(nb));
	addPage(new d2d::LibraryFontPage(nb));
	addPage(new eparticle3d::LibraryPage(nb));
	addPage(new eparticle2d::LibraryPage(nb));
	addPage(new eejoy2d::LibraryPage(nb));
	addPage(new eterrain2d::LibraryPage(nb));
	addPage(new etexture::LibraryPage(nb));
	addPage(new eicon::LibraryPage(nb));
}

} // ecomplex