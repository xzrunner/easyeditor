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
#include <easyshadow.h>
#include <easysketch.h>
#include <easytext.h>

namespace ecomplex
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: ee::LibraryPanel(parent)
{
	wxWindow* nb = GetNotebook();
	AddPage(new ee::LibraryImagePage(nb));
	AddPage(new LibraryPage(nb));
	AddPage(new libanim::LibraryPage(nb));
	AddPage(new escale9::LibraryPage(nb));
	AddPage(new etext::LibraryPage(nb));
	AddPage(new eparticle3d::LibraryPage(nb));
	AddPage(new etexture::LibraryPage(nb));
	AddPage(new eicon::LibraryPage(nb));
	AddPage(new libshape::LibraryPage(nb));

	AddPage(new ee::LibraryFontBlankPage(nb));
	AddPage(new emesh::LibraryPage(nb));
	AddPage(new ee::LibraryScriptsPage(nb));
	AddPage(new eparticle2d::LibraryPage(nb));
	AddPage(new eejoy2d::LibraryPage(nb));
	AddPage(new eterrain2d::LibraryPage(nb));
	AddPage(new eshadow::LibraryPage(nb));
	AddPage(new libsketch::LibraryPage(nb));
}

} // ecomplex